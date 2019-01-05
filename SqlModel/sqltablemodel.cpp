#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject *parent):
    QSqlTableModel(parent)
{
    connect(this, &SqlTableModel::connectionNameChanged, this, &SqlTableModel::_updateCurrentQuery);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsInserted, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsRemoved, this, &SqlTableModel::rowCountChanged);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsInserted, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsRemoved, this, &SqlTableModel::columnCountChanged);

    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

SqlTableModel::SqlTableModel(const QString &connectionName, QObject *parent):
    QSqlTableModel (parent, GET_DATABASE(connectionName))
{
    connect(this, &SqlTableModel::connectionNameChanged, this, &SqlTableModel::_updateCurrentQuery);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsInserted, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsRemoved, this, &SqlTableModel::rowCountChanged);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsInserted, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsRemoved, this, &SqlTableModel::columnCountChanged);

    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QString SqlTableModel::connectionName() const
{
    return m_connectionName;
}

void SqlTableModel::setConnectionName(const QString &name)
{
    m_connectionName = name;
    emit connectionNameChanged();
}

QString SqlTableModel::_query() const
{
    return query().executedQuery();
}

int SqlTableModel::_rowCount() const
{
    return rowCount();
}

int SqlTableModel::_columnCount() const
{
    return columnCount();
}

void SqlTableModel::_initRoles()
{
    m_rolesNames.clear();
    m_rolesNames[Qt::DisplayRole] = "display";

    QSqlRecord rcd = record();
    for (int i=0;i<rcd.count();++i)
    {
        m_rolesNames[Qt::UserRole+i] = QVariant::fromValue(rcd.fieldName(i)).toByteArray();
    }
}

QHash<int, QByteArray> SqlTableModel::roleNames() const
{
    return m_rolesNames;
}

QVariant SqlTableModel::data(const QModelIndex &item, int role) const
{
    if (item.column()==0 && role>=Qt::UserRole)
    {
        return QSqlTableModel::data(index(item.row(), role-Qt::UserRole, item.parent()), Qt::DisplayRole);
    }

    return QSqlTableModel::data(item, role);
}

void SqlTableModel::queryChange()
{
    QSqlDatabase db = GET_DATABASE(m_connectionName);

    if (!lastError().isValid())
    {
        _initRoles();

        if (!db.driver()->hasFeature(QSqlDriver::QuerySize))
        {
            // fetch all data
            while (canFetchMore())
                fetchMore();
        }
    }

    emit queryChanged();
}

void SqlTableModel::_setQuery(const QString &cmd)
{
    if (m_connectionName.isEmpty())
    {
        qDebug() << "invalid connection name";

        // store sql request to execute when connection name will be available
        m_tmpQuery = cmd;

        return;
    }

    QSqlDatabase db = GET_DATABASE(m_connectionName);
    if (!db.isValid() || !db.isOpen())
    {
        qCritical() << "invalid database" << m_connectionName;

        // store sql request to execute when connection name will be ready
        m_tmpQuery = cmd;

        return;
    }

    QSqlQueryModel::setQuery(cmd, db);
}

void SqlTableModel::_updateCurrentQuery()
{
    if (m_connectionName.isEmpty())
    {
        clear();
        return;
    }

    if (!m_tmpQuery.isEmpty())
    {
        QString cmd = m_tmpQuery;
        m_tmpQuery.clear();
        _setQuery(cmd);
    }
    else
    {
        QString tmpQuery = query().executedQuery();
        if (!tmpQuery.isEmpty())
            _setQuery(tmpQuery);
        else
            clear();
    }
}

void SqlTableModel::reload()
{
    _updateCurrentQuery();
}

void SqlTableModel::remove(const int &index, const int &count)
{
    QString tmpQuery = _query();

    removeRows(index, count);

    submitAll();

    if (tmpQuery != _query())
        _setQuery(tmpQuery);
}
