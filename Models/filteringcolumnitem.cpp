#include "filteringcolumnitem.h"

FilteringColumnItem::FilteringColumnItem(QObject *parent) :
    ListItem(parent),
    m_name(),
    m_used(false)
{
}

FilteringColumnItem::FilteringColumnItem(const QString &name, QObject *parent) :
    ListItem(parent),
    m_name(name),
    m_used(false)
{
}

QHash<int, QByteArray> FilteringColumnItem::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole]             = "id";
    roles[NameRole]           = "name";
    roles[UsedRole]           = "used";
    return roles;
}

QVariant FilteringColumnItem::data(int role) const
{
    switch(role) {
    case IdRole:
        return QVariant::fromValue(id());
    case NameRole:
        return QVariant::fromValue(m_name);
    case UsedRole:
        return QVariant::fromValue(m_used);
    default:
        return QVariant::Invalid;
    }
}

bool FilteringColumnItem::setData(const QVariant &value, const int &role)
{
    QVector<int> roles;
    roles << role;

    if (role == NameRole)
    {
        m_name = value.toString();
        emit itemChanged(roles);
        return true;
    }

    if (role == UsedRole)
    {
        m_used = value.toBool();
        emit itemChanged(roles);
        return true;
    }

    qWarning() << "invalid role" << role;
    return false;
}
