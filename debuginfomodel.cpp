#include "debuginfomodel.h"

DebugInfoModel::DebugInfoModel(QObject *parent):
    ListModel(new DebugInfoItem, parent)
{
    connect(this, &DebugInfoModel::updateAllItemsSignal, this, &DebugInfoModel::_updateAllItems);
    connect(this, &DebugInfoModel::updateItemSignal, this, &DebugInfoModel::_updateItem);
}

void DebugInfoModel::_updateAllItems()
{
    QHash<QString, QList<QObject*>>::iterator i;
    for (i = h_objects.begin(); i != h_objects.end(); ++i)
       _updateItem(i.key());
}

void DebugInfoModel::_updateItem(const QString &className)
{
    if (h_objects.contains(className))
    {
        auto item = qobject_cast<DebugInfoItem*>(find(className));
        if (item)
        {
            // update item
            item->setData(h_objects[className].size(), DebugInfoItem::CounterRole);
        }
        else
        {
            // add new item
            item = new DebugInfoItem(this);
            item->setData(className, DebugInfoItem::NameRole);
            item->setData(h_objects[className].size(), DebugInfoItem::CounterRole);
            appendRow(item);
        }
    }
    else
    {
        qCritical() << "unable to update debug item" << className;
    }
}

void DebugInfoModel::add_object(QObject *obj)
{
    if (!alive_objects.contains(obj))
    {
        alive_objects << obj;

        QString className = obj->metaObject()->className();
        if (!h_objects.contains(className))
            h_objects[className] = QList<QObject*>();

        h_objects[className] << obj;

        emit updateItemSignal(className);
    }
    else
    {
        qCritical() << "unable to add" << obj << "already in the list.";
    }
}

void DebugInfoModel::remove_object(QObject *obj)
{
    alive_objects.removeAll(obj);

    QHash<QString, QList<QObject*>>::iterator i;
    for (i = h_objects.begin(); i != h_objects.end(); ++i)
    {
        if (i.value().contains(obj))
        {
            i.value().removeAll(obj);
            emit updateItemSignal(i.key());
        }
    }
}

void DebugInfoModel::display_alive_objects()
{
    if (alive_objects.isEmpty())
    {
        qInfo() << "NO ALIVE OBJECTS.";
    }
    else
    {
        qInfo() << "ALIVE OBJECTS:";
        for (auto elt : alive_objects)
            qInfo() << "   " << elt;
    }

    qInfo() << "";
}

int DebugInfoModel::count_alive_objects(const QString &className)
{
    if (h_objects.contains(className))
        return h_objects[className].size();

    return 0;
}

QHash<QString, int> DebugInfoModel::count_alive_objects_by_className()
{
    QHash<QString, int> res;
    QHash<QString, QList<QObject*>>::iterator i;
    for (i = h_objects.begin(); i != h_objects.end(); ++i)
        res[i.key()] = i.value().size();
    return res;
}

QAbstractItemModel *DebugInfoModel::detailsModel(const QString &className)
{
    auto model = new QStringListModel(this);
    QStringList list;
    if (h_objects.contains(className))
    {
        for (QObject *elt : h_objects[className])
        {
            QString objectString;
            if (elt->parent())
                objectString = QString("%1 parent(%2)").arg(elt->metaObject()->className(), elt->parent()->metaObject()->className());
            else
                objectString = QString("%1 parent(null)").arg(elt->metaObject()->className());
            list << objectString;
        }
    }
    model->setStringList(list);
    return model;
}
