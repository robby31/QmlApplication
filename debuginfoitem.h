#ifndef DEBUGINFOITEM_H
#define DEBUGINFOITEM_H

#include <QHash>
#include <QVariant>
#include "Models/listitem.h"

class DebugInfoItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
      NameRole = Qt::UserRole+1,
      CounterRole
    };

    explicit DebugInfoItem(QObject *parent = Q_NULLPTR);

    QString id() const Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(int role) const Q_DECL_OVERRIDE;
    bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;

private:
    QString m_name;
    int m_counter = 0;
};

#endif // DEBUGINFOITEM_H
