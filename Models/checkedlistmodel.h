#ifndef CHECKEDLISTMODEL_H
#define CHECKEDLISTMODEL_H

#include "libqmlapplication_global.h"
#include "listmodel.h"
#include "checkeditem.h"

class QMLAPPLICATIONSHARED_EXPORT CheckedListModel : public ListModel
{
    Q_OBJECT

public:
    explicit CheckedListModel(QObject *parent = 0);

    bool allChecked();

    CheckedItem *findByRole(const QVariant &value, const int &role);

signals:

public slots:

};

#endif // CHECKEDLISTMODEL_H
