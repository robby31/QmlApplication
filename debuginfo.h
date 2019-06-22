#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QDebug>
using namespace std;

#include "debuginfomodel.h"

class DebugInfo
{

public:
    DebugInfo(DebugInfo const&) = delete;
    void operator=(DebugInfo const&)  = delete;

    static void add_object(QObject *obj);
    static void remove_object(QObject *obj);

    static void display_alive_objects();
    static int count_alive_objects(const QString &className = QString());

    static DebugInfoModel *model;

private:
    DebugInfo() = default;
};

#endif // DEBUGINFO_H

