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
        for (int row=0; row<m_list.size(); ++row)
        {
            if (!m_list.at(row)->data(roleIndex).toBool())
                return false;
        }
    }

    return true;
}

CheckedItem *CheckedListModel::findByRole(const QVariant &value, const int &role)
{
    for (int row=0; row<m_list.size(); ++row)
    {
        auto item = qobject_cast<CheckedItem*>(m_list.at(row));
        if (item && item->data(role) == value)
            return item;
    }

    return Q_NULLPTR;
}
