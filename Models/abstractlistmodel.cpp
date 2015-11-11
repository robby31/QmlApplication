#include "abstractlistmodel.h"

AbstractListModel::AbstractListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    connect(this, SIGNAL(modelReset()), this, SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(modelReset()), this, SIGNAL(columnCountChanged()));
}

QString AbstractListModel::role(int columnNumber) const
{
    QHash<int, QByteArray> roles = roleNames();

    if (roles.contains(Qt::UserRole+columnNumber+1))
        return roles[Qt::UserRole+columnNumber+1];
    else
        return QString();
}

QVariant AbstractListModel::dataByStringRole(const QModelIndex &index, QString role) const
{
    return data(index, roleNames().key(role.toLatin1()));
}

QVariant AbstractListModel::get(int i, QString role) const
{
    return dataByStringRole(indexFromRow(i), role);
}

QModelIndex AbstractListModel::indexFromRow(const int row) const
{
    return index(row, 0);
}

int AbstractListModel::findRow(const QVariant &value, const int &role) const
{
    for (int row=0; row<rowCount(); ++row)
    {
        if (data(indexFromRow(row), role) == value)
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
