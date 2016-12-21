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

    explicit FilteringColumnItem(QObject *parent = 0);
    explicit FilteringColumnItem(const QString &name, QObject *parent = 0);

    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, const int &role);
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:

private:
    QString m_name;
    bool m_used;
};

#endif // FILTERINGCOLUMNITEM_H
