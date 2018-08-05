#include "basesqllistmodel.h"

BaseSqlListModel::BaseSqlListModel(QObject *parent) :
    AbstractListModel(parent),
    mRecords(),
    mconnectionName(),
    mStringQuery(),
    mSqlQuery(),
    mRoles(),
    m_tablename(),
    m_filtercmd(),
    m_customRoles()
{
    connect(this, SIGNAL(isFilteredChanged()), this, SLOT(update_query()));
}

int BaseSqlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRecords.count();
}

int BaseSqlListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRoles.size();
}

QVariant BaseSqlListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            if (section >= 0 && section < columnCount())
                return mRoles[Qt::UserRole+section];
            else
                return QVariant::Invalid;
        }
    }

    return AbstractListModel::headerData(section, orientation, role);
}

QVariant BaseSqlListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < mRecords.count())
    {
        if (role == Qt::DisplayRole)
        {
            if (index.column() >= 0 && index.column() < columnCount())
                return mRecords.at(index.row()).value(index.column());
        }
        else if (role == Qt::EditRole)
        {
            if (index.column() >= 0 && index.column() < columnCount())
                return mRecords.at(index.row()).value(index.column());
        }
        else if (role - Qt::UserRole >= 0)
        {
            return mRecords.at(index.row()).value(role - Qt::UserRole);
        }
    }

    return QVariant::Invalid;
}

QHash<int, QByteArray> BaseSqlListModel::roleNames() const
{
    return mRoles;
}

bool BaseSqlListModel::add_role(const QByteArray &roleName)
{
    if (!mRoles.values().contains(roleName))
    {
        mRoles.insert(Qt::UserRole+mRoles.size(), roleName);
        return true;
    }
    else
    {
        qWarning() << "unable to add role" << roleName;
        return false;
    }
}

void BaseSqlListModel::setQuery(const QString &query)
{
    mStringQuery = query;
    update_query();
}

void BaseSqlListModel::setTablename(const QString &name)
{
    m_tablename = name;
    emit tablenameChanged();
}

QString BaseSqlListModel::query()
{
    return mStringQuery;
}

QString BaseSqlListModel::tablename()
{
    return m_tablename;
}

QString BaseSqlListModel::lastError()
{
    return mSqlQuery.lastError().text();
}

bool BaseSqlListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QVector<int> roles;
    roles << role;

    QString strRole = QVariant::fromValue(mRoles[role]).toString();
    if (!m_tablename.isEmpty() && index.row() >= 0 && index.row() < mRecords.count())
    {
        QSqlDatabase db = GET_DATABASE(mconnectionName);

        QSqlIndex primaryIndex = db.primaryIndex(m_tablename);
        if (primaryIndex.count() == 0)
        {
            qCritical() << "setData unable to find primary index for table" << m_tablename;
        }
        else
        {
            QString primaryKey = primaryIndex.fieldName(0);

            QSqlRecord record = mRecords.at(index.row());

            QVariant primaryKeyData = record.value(primaryKey);
            if (primaryKeyData.isValid())
            {
                QSqlQuery query(GET_DATABASE(mconnectionName));
                query.prepare(QString("UPDATE %2 SET %1=:value WHERE %3=:id").arg(strRole).arg(m_tablename).arg(primaryKey));
                if (strcmp(value.typeName(), "QDateTime") == 0)
                    query.bindValue(":value", QVariant::fromValue(value.toDate()));
                else
                    query.bindValue(":value", value);
                query.bindValue(":id", primaryKeyData);
                if (query.exec())
                {
                    if (mSqlQuery.exec() && mSqlQuery.seek(index.row()))
                    {
                        if (mSqlQuery.record().value(primaryKey).isNull() or mSqlQuery.record().value(primaryKey).toInt() != mRecords[index.row()].value(primaryKey).toInt())
                            qCritical() << "setData unable to update data" << primaryKey << mSqlQuery.record().value(primaryKey).toInt() << mRecords[index.row()].value(primaryKey).toInt();
                        else
                            mRecords[index.row()] = mSqlQuery.record();

                        emit dataChanged(index, index, roles);

                        return true;
                    }
                    else
                    {
                        qCritical() << "setData unable to update data" << mSqlQuery.lastError().text();
                    }
                }
                else
                {
                    qCritical() << "setData" << index << value << role << query.lastQuery() << query.lastError();
                }
            }
            else
            {
                qCritical() << "setData invalid primary key value" << primaryKey << record;
            }
        }
    }

    if (m_tablename.isEmpty())
        qCritical() << "<SqlListModel>: unable to setData, tablename is empty.";

    return false;
}

void BaseSqlListModel::resetModel()
{
    setQuery(mSqlQuery.lastQuery());
}

void BaseSqlListModel::reload()
{
    QString lastQuery = mSqlQuery.lastQuery();
    if (!lastQuery.isEmpty())
    {
        QElapsedTimer timer;
        timer.start();

        mSqlQuery.clear();
        mSqlQuery.setForwardOnly(true);
        qint64 execQueryPerfo = -1;
        int query_size = 0;
        int oldSize = mRecords.size();
        QList<T_DATACHANGED> l_dataToUpdate;

        qDebug() << "RELOAD" << lastQuery;
        if (mSqlQuery.exec(lastQuery))
        {
            execQueryPerfo = timer.elapsed();
            while (mSqlQuery.next())
            {
                int row = mSqlQuery.at();
                query_size = row + 1;

                // check roles for first index
                if (row == 0 && row < mRecords.size())
                {
                    if (mSqlQuery.record().count() != mRecords.at(row).count())
                    {
                        qCritical() << "roles shall be updated, cannot reload the model.";
                        break;
                    }

                    for (int i=0; i<mSqlQuery.record().count();++i)
                    {
                        if (mRecords.at(row).fieldName(i) != mSqlQuery.record().fieldName(i))
                        {
                            qCritical() << "roles shall be updated, cannot reload the model" << i << mRecords.at(row).fieldName(i) << mSqlQuery.record().fieldName(i);
                            break;
                        }
                    }
                }

                // check data
                if (row>=0 && row<mRecords.size())
                {
                    QList<int> columnToUpdate;
                    QVector<int> rolesToUpdate;
                    for (int i=0; i<mSqlQuery.record().count();++i)
                    {
                        if (mRecords.at(row).field(i).value() != mSqlQuery.record().field(i).value())
                        {
                            columnToUpdate << i;
                            rolesToUpdate << Qt::UserRole+i;
                        }
                    }

                    if (!columnToUpdate.isEmpty())
                    {
                        mRecords[row] = mSqlQuery.record();

                        auto min_max = std::minmax_element(columnToUpdate.begin(), columnToUpdate.end());
                        int minColumn = *min_max.first;
                        int maxColumn = *min_max.second;
                        qDebug() << "index" << row << "column to update" << minColumn << maxColumn << columnToUpdate << "roles to update" << rolesToUpdate;

                        T_DATACHANGED dataToUpdate;
                        dataToUpdate.topLeft = index(row, minColumn);
                        dataToUpdate.bottomRight = index(row, maxColumn);
                        dataToUpdate.roles = rolesToUpdate;
                        l_dataToUpdate.append(dataToUpdate);
                    }
                }
                else
                {
                    // new row to add
                    mRecords.append(mSqlQuery.record());
                }
            }

            if (mRecords.size() != oldSize && oldSize < mRecords.size())
            {
                // rows have been added
                beginInsertRows(QModelIndex(), oldSize, mRecords.size()-1);
                endInsertRows();
            }

            if (mRecords.size() > query_size)
            {
                // rows shall be removed
                beginRemoveRows(QModelIndex(), query_size, mRecords.size()-1);
                while (mRecords.size() > query_size)
                    mRecords.removeLast();
                endRemoveRows();
            }

            if (mRecords.size() != query_size)
            {
                qCritical() << "row count changed" << mRecords.size() << query_size;
            }

            foreach (T_DATACHANGED elt, l_dataToUpdate)
                emit dataChanged(elt.topLeft, elt.bottomRight, elt.roles);
        }
        else
        {
            qCritical() << "query failed in reload:" << lastQuery;
            qCritical() << mSqlQuery.lastError().text();
        }

        if (timer.elapsed() > 250)
            qDebug() << "perfo reload query" << timer.elapsed() << "query executed in" << execQueryPerfo << "size" << rowCount() << mSqlQuery.lastQuery();

        emit queryChanged();
        emit lastErrorChanged();
    }
    else
    {
        qCritical() << "unable to reload model, query is empty.";
    }
}

void BaseSqlListModel::setFilter(const QString &cmd)
{
    m_filtercmd = cmd;
    emit isFilteredChanged();
}

void BaseSqlListModel::addCustomRole(const QByteArray &role)
{
    if (!m_customRoles.contains(role))
        m_customRoles << role;
}

void BaseSqlListModel::update_query()
{
    QElapsedTimer timer;
    timer.start();

    QString filteredStringQuery;
    if (filterCmd().isEmpty())
        filteredStringQuery = mStringQuery;
    else
        filteredStringQuery = QString("SELECT * from (%1) AS filtered_query WHERE %2").arg(mStringQuery).arg(filterCmd());

    if (!m_orderBy.isEmpty())
        filteredStringQuery += QString(" ORDER BY %1").arg(m_orderBy);

    beginResetModel();
    mRoles.clear();
    mRecords.clear();

    qint64 execQueryPerfo = -1;
    if (mSqlQuery.driver()->isOpen() && !filteredStringQuery.isEmpty())
    {
        mSqlQuery.clear();
        mSqlQuery.setForwardOnly(true);
        if (mSqlQuery.exec(filteredStringQuery))
        {
            execQueryPerfo = timer.elapsed();
            while (mSqlQuery.next()){

                if (!mSqlQuery.at())
                {
                    for (int i=0; i<mSqlQuery.record().count();++i) {
                        mRoles.insert(Qt::UserRole+i, mSqlQuery.record().fieldName(i).toUtf8());
                    }
                }

                mRecords.append(mSqlQuery.record());
            }
        }
        else
        {
            qWarning() << "query failed" << filteredStringQuery;
            qWarning() << mSqlQuery.lastError().text();
        }
    }
    else
    {
        mSqlQuery.clear();
    }

    // add custom roles
    foreach (const QByteArray &role, m_customRoles)
        add_role(role);

    endResetModel();

    emit queryChanged();
    emit lastErrorChanged();

    if (timer.elapsed() > 250)
        qDebug() << "perfo query" << timer.elapsed() << "query executed in" << execQueryPerfo << "size" << rowCount() << mSqlQuery.lastQuery();
}

QString BaseSqlListModel::connectionName()
{
    return mconnectionName;
}

void BaseSqlListModel::setConnectionName(const QString &name)
{
    mconnectionName = name;
    emit connectionNameChanged();

    if (GET_DATABASE(mconnectionName).driverName() == "SQLITE" && getPragma("foreign_keys").toBool())
        readForeignKeys();

    mSqlQuery = QSqlQuery(GET_DATABASE(mconnectionName));
    setQuery(mStringQuery);
}

bool BaseSqlListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QSqlDatabase db = GET_DATABASE(mconnectionName);

    if (m_tablename.isEmpty())
    {
        qCritical() << "tablename is not defined" << m_tablename;
        return false;
    }
    else
    {
        if (db.transaction())
        {
            QSqlIndex primaryIndex = db.primaryIndex(m_tablename);
            if (primaryIndex.count() == 0)
            {
                qCritical() << "unable to find primary index for table" << m_tablename;
                db.rollback();
                return false;
            }
            else
            {
                QString primaryKey = primaryIndex.fieldName(0);

                for (int i=0;i<count;++i)
                {
                    QSqlRecord record = mRecords.at(row);
                    QVariant primaryKeyData = record.value(primaryKey);
                    if (primaryKeyData.isValid())
                    {
                        QSqlQuery query(db);
                        query.prepare(QString("DELETE FROM %1 WHERE %2=:id").arg(m_tablename).arg(primaryKey));
                        query.bindValue(":id", primaryKeyData);
                        if (!query.exec())
                        {
                            qCritical() << query.lastError().text();
                            db.rollback();
                            return false;
                        }
                        else
                        {
                            beginRemoveRows(parent, row, row+count-1);
                            mRecords.removeAt(row);
                            endRemoveRows();
                        }
                    }
                    else
                    {
                        qCritical() << "invalid primarykey value" << primaryKey << primaryKeyData;
                        db.rollback();
                        return false;
                    }
                }
            }

            return db.commit();
        }
        else
        {
            qCritical() << db.lastError().text();
            return false;
        }
    }
}

bool BaseSqlListModel::remove(int row)
{
    return removeRow(row);
}

QVariant BaseSqlListModel::getPragma(const QString &param)
{
    QSqlDatabase db = GET_DATABASE(mconnectionName);

    if (db.isValid() && db.isOpen())
    {
        QSqlQuery query(db);
        if (!query.exec(QString ("pragma %1;").arg(param)))
        {
            qCritical() << "unable to get PRAGMA" << param << ":" << query.lastError().text();
            return QVariant();
        }
        else
        {
            if (query.next())
                return query.value(0);
            else
                return QVariant();
        }
    }
    else
    {
        qCritical() << QThread::currentThread() << "database is not valid or not open, unable to get pragma" << param;
        return QVariant();
    }
}

bool BaseSqlListModel::readForeignKeys()
{
    QSqlDatabase db = GET_DATABASE(mconnectionName);

    if (db.driverName() == "SQLITE")
    {
        if (db.isValid() && db.isOpen())
        {
            m_foreignKeys.clear();

            QSqlQuery query(db);
            foreach(QString tableName, db.tables())
            {
                query.exec(QString("pragma foreign_key_list(%1);").arg(tableName));
                while (query.next())
                {
                    QHash<QString, QString> tmp;
                    tmp["table"] = query.value("table").toString();
                    tmp["to"] = query.value("to").toString();
                    m_foreignKeys[tableName][query.value("from").toString()] = tmp;
                }
            }

            qDebug() << "read foreign keys" << m_foreignKeys;
            return true;
        }
        else
        {
            qCritical() << "database (name ="<< mconnectionName << ") is not valid or open, unable to read foreign keys.";
            return false;
        }
    }
    else
    {
        qWarning() << "foreign keys not supported" << db.driverName();
        return true;
    }
}

int BaseSqlListModel::append(const QVariantMap &data)
{
    QStringList keys = data.keys();
    QStringList params;
    foreach (QString param, keys)
        params << QString(":%1").arg(param);

    QSqlQuery query(GET_DATABASE(connectionName()));

    QString cmd = QString("INSERT INTO %3 (%1) VALUES(%2)").arg(keys.join(",")).arg(params.join(",")).arg(tablename());
    query.prepare(cmd);

    foreach (QString param, keys)
        query.bindValue(QString(":%1").arg(param), data[param]);

    if (!query.exec())
    {
        qCritical() << "invalid query" << query.lastError().text();
        return -1;
    }
    else
    {
        return query.lastInsertId().toInt();
    }
}

QString BaseSqlListModel::orderBy() const
{
    return m_orderBy;
}

void BaseSqlListModel::setOrderBy(const QString &cmd)
{
    m_orderBy = cmd;
    emit orderByChanged();
}
