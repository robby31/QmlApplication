#include "checkedlistmodel.h"

CheckedListModel::CheckedListModel(QObject *parent) :
    ListModel(new CheckedItem, parent)
{
}

bool CheckedListModel::allChecked()
{
    int roleIndex = roleNames().key("checked");

    if (roleNames().contains(roleIndex))
    {
        for (auto item = m_list.constBegin(); item != m_list.constEnd(); ++item)
        {
            auto checked_item = qobject_cast<CheckedItem*>(*item);
            if (!checked_item->data(roleIndex).toBool())
                return false;
        }
    }

    return true;
}

CheckedItem *CheckedListModel::findByRole(const QVariant &value, const int &role)
{
    for (auto item = m_list.constBegin(); item != m_list.constEnd(); ++item)
    {
        auto checked_item = qobject_cast<CheckedItem*>(*item);
        if (checked_item && checked_item->data(role) == value)
            return checked_item;
    }

    return Q_NULLPTR;
}
