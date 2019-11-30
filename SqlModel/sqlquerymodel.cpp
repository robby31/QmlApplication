#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(QObject *parent):
    QSqlQueryModel(parent)
{
    DebugInfo::add_object(this);

    connect(this, &SqlQueryModel::connectionNameChanged, this, &SqlQueryModel::_updateCurrentQuery);

    connect(this, &SqlQueryModel::modelReset, this, &SqlQueryModel::rowCountChanged);
    connect(this, &SqlQueryModel::rowsInserted, this, &SqlQueryModel::rowCountChanged);
    connect(this, &SqlQueryModel::rowsRemoved, this, &SqlQueryModel::rowCountChanged);

    connect(this, &SqlQueryModel::modelReset, this, &SqlQueryModel::columnCountChanged);
    connect(this, &SqlQueryModel::columnsInserted, this, &SqlQueryModel::columnCountChanged);
    connect(this, &SqlQueryModel::columnsRemoved, this, &SqlQueryModel::columnCountChanged);
}

QString SqlQueryModel::connectionName() const
{
    return m_connectionName;
}

void SqlQueryModel::setConnectionName(const QString &name)
{
    m_connectionName = name;
    emit connectionNameChanged();
}

QString SqlQueryModel::_query() const
{
    return query().executedQuery();
}

int SqlQueryModel::_rowCount() const
{
    return rowCount();
}

int SqlQueryModel::_columnCount() const
{
    return columnCount();
}

void SqlQueryModel::_initRoles()
{
    m_rolesNames.clear();
    m_rolesNames[Qt::DisplayRole] = "display";

    QSqlRecord rcd = record();
    for (int i=0;i<rcd.count();++i)
    {
        m_rolesNames[Qt::UserRole+i] = QVariant::fromValue(rcd.fieldName(i)).toByteArray();
    }
}

QHash<int, QByteArray> SqlQueryModel::roleNames() const
{
    return m_rolesNames;
}

QVariant SqlQueryModel::data(const QModelIndex &item, int role) const
{
    if (item.column()==0 && role>=Qt::UserRole)
    {
        return QSqlQueryModel::data(index(item.row(), role-Qt::UserRole, item.parent()), Qt::DisplayRole);
    }

    return QSqlQueryModel::data(item, role);
}

void SqlQueryModel::queryChange()
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

void SqlQueryModel::_setQuery(const QString &cmd)
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

    setQuery(cmd, db);
}

void SqlQueryModel::_updateCurrentQuery()
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

void SqlQueryModel::reload()
{
    _updateCurrentQuery();
}

QVariantMap SqlQueryModel::get(const int &index)
{
    QVariantMap res;

    if (index >=0 && index < rowCount())
    {
        QSqlRecord data = record(index);
        for (int i=0;i<data.count();++i)
            res[data.fieldName(i)] = data.value(i);
    }

    return res;
}
