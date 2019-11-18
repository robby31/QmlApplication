#include "debuginfo.h"

DebugInfoModel *DebugInfo::model = new DebugInfoModel();

void DebugInfo::add_object(QObject *obj)
{
    model->add_object(obj);
}

void DebugInfo::display_alive_objects()
{
    model->display_alive_objects();
}

int DebugInfo::count_alive_objects(const QString &className)
{
    return model->count_alive_objects(className);
}
