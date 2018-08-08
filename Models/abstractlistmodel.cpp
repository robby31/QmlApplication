#include "abstractlistmodel.h"

AbstractListModel::AbstractListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    connect(this, SIGNAL(modelReset()), this, SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));

    connect(this, SIGNAL(modelReset()), this, SIGNAL(columnCountChanged()));
    connect(this, SIGNAL(columnsInserted(QModelIndex,int,int)), this, SIGNAL(columnCountChanged()));
    connect(this, SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SIGNAL(columnCountChanged()));
}

QString AbstractListModel::role(int columnNumber) const
{
    QHash<int, QByteArray> roles = roleNames();

    if (roles.contains(Qt::UserRole+columnNumber+1))
        return roles[Qt::UserRole+columnNumber+1];

    return QString();
}

QVariant AbstractListModel::dataByStringRole(const QModelIndex &index, const QString &role) const
{
    int intRole = roleNames().key(role.toLatin1());

    if (!roleNames().contains(intRole))
        qCritical() << "invalid role" << role << intRole << roleNames().keys();

    return data(index, intRole);
}

QVariantMap AbstractListModel::get(const int &index)
{
    QVariantMap res;

    if (index >=0 && index < rowCount())
    {
        QHash<int, QByteArray> roles = roleNames();
        foreach (int role, roles.keys())
            res[roles[role]] = data(indexFromRow(index), role);
    }

    return res;
}

QVariant AbstractListModel::get(int i, const QString &role) const
{
    return dataByStringRole(indexFromRow(i), role);
}

QModelIndex AbstractListModel::indexFromRow(const int row) const
{
    return index(row, 0);
}

int AbstractListModel::findRow(const QVariant &value, const QString &role) const
{
    int intRole = roleNames().key(role.toLatin1());

    if (!roleNames().contains(intRole))
        qCritical() << "invalid role" << role << intRole << roleNames().keys();

    for (int row=0; row<rowCount(); ++row)
    {
        if (data(indexFromRow(row), intRole) == value)
            return row;
    }

    return -1;
}

bool AbstractListModel::setDataByStringRole(const int row, const QVariant &value, const QString &role)
{
    return setDataByStringRole(indexFromRow(row), value, role);
}

bool AbstractListModel::setDataByStringRole(const QModelIndex &index, const QVariant &value, const QString &role)
{
    return setData(index, value, roleNames().key(role.toLatin1()));
}
