#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QVector>

class ListItem : public QObject
{
    Q_OBJECT

public:
    explicit ListItem(QObject* parent = 0);

    virtual QString id() const { return QString("%1").arg(quintptr(this)); }

    virtual QHash<int, QByteArray> roleNames() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual bool setData(const QVariant &value, const int &role) = 0;

signals:
    void itemChanged(const QVector<int> &roles = QVector<int>());
};

#endif // LISTITEM_H
