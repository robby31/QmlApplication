#ifndef CHECKEDITEM_H
#define CHECKEDITEM_H

#include <QDebug>
#include "Models/listmodel.h"

class CheckedItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole+1,
        NameRole,
        CheckedRole
    };

    explicit CheckedItem(QObject *parent=Q_NULLPTR);

    QVariant data(int role) const Q_DECL_OVERRIDE;
    bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    QString m_name;
    bool m_checked = true;
};

#endif // CHECKEDITEM_H
