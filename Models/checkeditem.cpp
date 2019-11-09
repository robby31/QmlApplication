#include "checkeditem.h"

CheckedItem::CheckedItem(QObject *parent):
    ListItem(parent),
    m_name()
{
}

QHash<int, QByteArray> CheckedItem::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole]                   = "id";
    roles[NameRole]                 = "name";
    roles[CheckedRole]              = "checked";
    return roles;
}

QVariant CheckedItem::data(int role) const
{
    switch(role) {
    case IdRole:
        return QVariant::fromValue(id());
    case NameRole:
        return QVariant::fromValue(m_name);
    case CheckedRole:
        return QVariant::fromValue(m_checked);
    default:
        return QVariant::Invalid;
    }
}

bool CheckedItem::setData(const QVariant &value, const int &role)
{
    QVector<int> roles;
    roles << role;

    switch (role) {
    case NameRole:
        m_name = value.toString();
        break;
    case CheckedRole:
        m_checked = value.toBool();
        break;
    default:
        qWarning() << "role" << role << "is readonly";
        return false;
    }
    emit itemChanged(roles);
    return true;
}
