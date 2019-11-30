#ifndef FILTERINGCOLUMNITEM_H
#define FILTERINGCOLUMNITEM_H

#include "listmodel.h"
#include <QDebug>

class FilteringColumnItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole+1,
        NameRole,
        UsedRole
    };

    explicit FilteringColumnItem(QObject *parent = Q_NULLPTR);
    explicit FilteringColumnItem(const QString &name, QObject *parent = Q_NULLPTR);

    QVariant data(int role) const Q_DECL_OVERRIDE;
    bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    QString m_name;
    bool m_used = false;
};

#endif // FILTERINGCOLUMNITEM_H
