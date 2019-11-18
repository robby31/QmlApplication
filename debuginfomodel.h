#ifndef DEBUGINFOMODEL_H
#define DEBUGINFOMODEL_H

#include "Models/listmodel.h"
#include "debuginfoitem.h"
#include <QStringListModel>
#include <QDebug>

class DebugInfoModel : public ListModel
{
    Q_OBJECT

public:    
    explicit DebugInfoModel(QObject *parent = Q_NULLPTR);

    void add_object(QObject *obj);
    void remove_object(QObject *obj);

    void display_alive_objects();
    int count_alive_objects(const QString &className = QString());
    QHash<QString, int> count_alive_objects_by_className();

signals:
    void updateAllItemsSignal();
    void updateItemSignal(const QString &className);

public slots:
    QAbstractItemModel* detailsModel(const QString &className);

private slots:
    void _updateAllItems();
    void _updateItem(const QString &className);
    void _objectDestroyed(QObject *obj);

private:
    QList<QObject*> alive_objects;
    QHash<QString, QList<QObject*> > h_objects;
};

#endif // DEBUGINFOMODEL_H
