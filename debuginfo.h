#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QDebug>

#include "debuginfomodel.h"

class DebugInfo
{

public:
    DebugInfo(DebugInfo const&) = delete;
    ~DebugInfo() = delete;
    DebugInfo& operator =(DebugInfo const&) = delete;
    DebugInfo(DebugInfo&&) = delete;
    DebugInfo& operator=(DebugInfo&&) = delete;

    static void add_object(QObject *obj);

    static void display_alive_objects();
    static int count_alive_objects(const QString &className = QString());

    static DebugInfoModel *model;

private:
    DebugInfo() = default;
};

#endif // DEBUGINFO_H

