#ifndef CHECKEDLISTMODEL_H
#define CHECKEDLISTMODEL_H

#include "listmodel.h"
#include "checkeditem.h"

class CheckedListModel : public ListModel
{
    Q_OBJECT

public:
    explicit CheckedListModel(QObject *parent = Q_NULLPTR);

    bool allChecked();

    CheckedItem *findByRole(const QVariant &value, const int &role);
};

#endif // CHECKEDLISTMODEL_H
