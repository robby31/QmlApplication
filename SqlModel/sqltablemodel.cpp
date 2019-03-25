#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject *parent):
    QSqlTableModel(parent)
{
    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsInserted, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsRemoved, this, &SqlTableModel::rowCountChanged);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsInserted, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsRemoved, this, &SqlTableModel::columnCountChanged);

    connect(this, &SqlTableModel::dataChanged, this, &SqlTableModel::dataUpdated);
}

SqlTableModel::SqlTableModel(const QString &connectionName, QObject *parent):
    QSqlTableModel (parent, GET_DATABASE(connectionName))
{
    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsInserted, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsRemoved, this, &SqlTableModel::rowCountChanged);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsInserted, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsRemoved, this, &SqlTableModel::columnCountChanged);

    connect(this, &SqlTableModel::dataChanged, this, &SqlTableModel::dataUpdated);
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
//    qWarning() << "DATA" << item << role;

    if (role >= Qt::UserRole && (role - Qt::UserRole) < columnCount())
    {
//        qWarning() << rowCount() << "custom data" << index(item.row(), role-Qt::UserRole, item.parent()) << Qt::DisplayRole << QSqlTableModel::data(index(item.row(), role-Qt::UserRole, item.parent()), Qt::DisplayRole);
        return QSqlTableModel::data(index(item.row(), role-Qt::UserRole, item.parent()), Qt::DisplayRole);
    }

//    qWarning() << rowCount() << "data" << item << role << QSqlTableModel::data(item, role);
    return QSqlTableModel::data(item, role);
}

void SqlTableModel::queryChange()
{
    _initRoles();

    emit queryChanged();
}

void SqlTableModel::_setQuery(const QString &cmd)
{
    m_customQuery = cmd;
}

bool SqlTableModel::remove(const int &index, const int &count)
{
    if (tableName().isEmpty())
    {
        qCritical() << "table name is empty, cannot remove row";
        return false;
    }

    if (!removeRows(index, count))
    {
        qCritical() << "cannot remove row" << index << count;
        return false;
    }

    if (!submitAll())
    {
        qCritical() << "cannot submit changes" << lastError();
        return false;
    }

    select();

    return true;
}

QString SqlTableModel::table() const
{
    return tableName();
}

void SqlTableModel::setTableName(const QString &name)
{
    setTable(name);
    emit tableChanged();
}

QString SqlTableModel::_filter() const
{
    return filter();
}

void SqlTableModel::setFilterTable(const QString &filter)
{
    setFilter(filter);
    emit filterChanged();
}

QString SqlTableModel::selectStatement() const
{
    if (!m_customQuery.isEmpty())
        return m_customQuery;

    return QSqlTableModel::selectStatement();
}

bool SqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qWarning() << rowCount() << this << "setData" << index << value << role;

    if (role >= Qt::UserRole && (role - Qt::UserRole) < columnCount())
        return QSqlTableModel::setData(createIndex(index.row(), role - Qt::UserRole), value, Qt::EditRole);

    return QSqlTableModel::setData(index, value, role);
}

void SqlTableModel::dataUpdated(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (m_submitFlag)
    {
        for (int row = topLeft.row(); row <= bottomRight.row(); ++row)
            m_submittedRows << row;
    }
}

bool SqlTableModel::submit()
{
    QElapsedTimer timer;
    timer.start();

    m_submittedRows.clear();
    m_submitFlag = true;

    bool res = QSqlTableModel::submit();

    m_submitFlag = false;

//    for (auto row : m_submittedRows)
//        selectRow(row);

    select();

    qWarning() << "submitted rows" << m_submittedRows << "PERFO" << timer.elapsed();

    return res;
}
