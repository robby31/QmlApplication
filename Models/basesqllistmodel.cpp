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
    return mSqlQuery.lastQuery();
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
        QSqlRecord record = mRecords.at(index.row());

        QSqlQuery query(GET_DATABASE(mconnectionName));
        query.prepare(QString("UPDATE %2 SET %1=:value WHERE id=:id").arg(strRole).arg(m_tablename));
        if (strcmp(value.typeName(), "QDateTime") == 0)
            query.bindValue(":value", QVariant::fromValue(value.toDate()));
        else
            query.bindValue(":value", value);
        query.bindValue(":id", record.value("id"));
        if (query.exec())
        {
            setQuery(mSqlQuery.lastQuery());
            return true;
        }
        else
        {
            qDebug() << query.lastError();
        }
    }

    if (tablename().isEmpty())
        qDebug() << "<SqlListModel>: unable to setData, tablename is empty.";

    return false;
}

void BaseSqlListModel::reload()
{
    setQuery(mSqlQuery.lastQuery());
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
        filteredStringQuery = QString("SELECT * from (%1) WHERE %2").arg(mStringQuery).arg(filterCmd());

    beginResetModel();
    mRoles.clear();
    mRecords.clear();

    if (mSqlQuery.driver()->isOpen() && !filteredStringQuery.isEmpty())
    {
        mSqlQuery.clear();
        if (mSqlQuery.exec(filteredStringQuery))
        {
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
        qDebug() << "perfo query" << timer.elapsed() << mSqlQuery.lastQuery();
}

QString BaseSqlListModel::connectionName()
{
    return mconnectionName;
}

void BaseSqlListModel::setConnectionName(const QString &name)
{
    mconnectionName = name;
    emit connectionNameChanged();

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

                QSqlQuery query(db);
                if (query.exec(QString("SELECT %1 FROM (%2)").arg(primaryKey).arg(mSqlQuery.executedQuery())))
                {
                    if (query.seek(row))
                    {
                        if (!query.exec(QString("DELETE FROM %1 WHERE %2>=%3 and %2<=%4").arg(m_tablename).arg(primaryKey).arg(query.record().value("id").toInt()).arg(query.record().value("id").toInt()+count-1)))
                        {
                            qCritical() << query.lastError().text();
                            db.rollback();
                            return false;
                        }
                        else
                        {
                            beginRemoveRows(parent, row, row+count-1);
                            endRemoveRows();
                        }
                    }
                    else
                    {
                        qCritical() << "unable to seek QSqlQuery" << query.lastError().text();
                        db.rollback();
                        return false;
                    }
                }
                else
                {
                    qCritical() << "unable to retrieve primary key values for removing data" << query.lastError().text();
                    db.rollback();
                    return false;
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
