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
}

SqlTableModel::SqlTableModel(const QString &connectionName, QObject *parent):
    QSqlTableModel(parent, GET_DATABASE(connectionName))
{
    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsInserted, this, &SqlTableModel::rowCountChanged);
    connect(this, &SqlTableModel::rowsRemoved, this, &SqlTableModel::rowCountChanged);

    connect(this, &SqlTableModel::modelReset, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsInserted, this, &SqlTableModel::columnCountChanged);
    connect(this, &SqlTableModel::columnsRemoved, this, &SqlTableModel::columnCountChanged);
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

    if (lastError().isValid())
        emit errorChanged();
}

QString SqlTableModel::error() const
{
    return lastError().text();
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
        emit errorChanged();
        return false;
    }

    return true;
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
    QString tmpQuery;

    if (!m_customQuery.isEmpty())
    {
        tmpQuery = m_customQuery;

        if (!join().isEmpty())
            tmpQuery += " " + join();

        if (!filter().isEmpty())
            tmpQuery += " WHERE " + filter();

        if (!orderByClause().isEmpty())
            tmpQuery += " " + orderByClause();
    }
    else
    {
        if (!join().isEmpty())
            qWarning() << "you shall set query to use JOIN";

        tmpQuery = QSqlTableModel::selectStatement();
    }

    if (!group().isEmpty())
        tmpQuery += " GROUP BY " + group();

    return tmpQuery;
}

bool SqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qWarning() << rowCount() << this << "setData" << index << value << role;

    if (role >= Qt::UserRole && (role - Qt::UserRole) < columnCount())
        return QSqlTableModel::setData(createIndex(index.row(), role - Qt::UserRole), value, Qt::EditRole);

    return QSqlTableModel::setData(index, value, role);
}

void SqlTableModel::setOrderClause(const QString &stmt)
{
    m_customOrder = stmt;
    emit orderByClauseChanged();
}

QString SqlTableModel::orderByClause() const
{
    if (!m_customOrder.isEmpty())
        return m_customOrder;

    return QSqlTableModel::orderByClause();
}

QString SqlTableModel::group() const
{
    return m_customGroup;
}

void SqlTableModel::setGroup(const QString &group)
{
    m_customGroup = group;
    emit groupChanged();
}

QString SqlTableModel::join() const
{
    return m_customJoin;
}

void SqlTableModel::setJoin(const QString &join)
{
    m_customJoin = join;
    emit joinChanged();
}

QVariantMap SqlTableModel::get(const int &index)
{
    QVariantMap res;

    if (index >=0 && index < rowCount())
    {
        QSqlRecord data = record(index);
        for (int i=0;i<data.count();++i)
            res[data.fieldName(i)] = data.value(i);

        if ((rowCount()-index<5) && canFetchMore())
            fetchMore();
    }

    return res;
}

int SqlTableModel::append(const QVariantMap &data)
{
    QStringList keys = data.keys();
    QStringList params;
    foreach (QString param, keys)
        params << QString(":%1").arg(param);

    QSqlQuery query(database());

    QString cmd = QString("INSERT INTO %3 (%1) VALUES(%2)").arg(keys.join(","), params.join(","), tableName());
    query.prepare(cmd);

    foreach (QString param, keys)
        query.bindValue(QString(":%1").arg(param), data[param]);

    if (!query.exec())
    {
        qCritical() << "invalid query" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}
