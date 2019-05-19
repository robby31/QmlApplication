#include "debuginfo.h"

QList<QObject*> DebugInfo::alive_objects;
//QList<DebugInfo::T_ELT*> DebugInfo::alive_elements;

void DebugInfo::add_object(QObject *obj)
{
    alive_objects << obj;
}

void DebugInfo::remove_object(QObject *obj)
{
    alive_objects.removeAll(obj);
}

void DebugInfo::display_alive_objects()
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

//        for (auto elt : alive_elements)
//            qInfo() << "   " << elt->className << elt->ptr;
    }

    qInfo() << "";
}

int DebugInfo::count_alive_objects(const QString &className)
{
    int res = 0;
    for (auto elt : alive_objects)
    {
        if (elt && (className.isNull() or elt->metaObject()->className() == className))
            ++res;
    }
    return res;
}
