#ifndef CHECKEDITEM_H
#define CHECKEDITEM_H

#include "libqmlapplication_global.h"
#include <QDebug>
#include "Models/listmodel.h"

class QMLAPPLICATIONSHARED_EXPORT CheckedItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole+1,
        NameRole,
        CheckedRole
    };

    explicit CheckedItem(QObject *parent=0);

    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, const int &role);
    QHash<int, QByteArray> roleNames() const;

private:
    QString m_name;
    bool m_checked;
};

#endif // CHECKEDITEM_H
