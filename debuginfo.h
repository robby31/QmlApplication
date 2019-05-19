#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QDebug>
using namespace std;

class DebugInfo
{
    typedef struct {
        void *ptr = Q_NULLPTR;
        QString className;
    } T_ELT;

public:
    DebugInfo(DebugInfo const&) = delete;
    void operator=(DebugInfo const&)  = delete;

    static void add_object(QObject *obj);
    static void remove_object(QObject *obj);

    static void display_alive_objects();
    static int count_alive_objects(const QString &className = QString());

private:
    DebugInfo() = default;
    static QList<QObject*> alive_objects;
};

#endif // DEBUGINFO_H

