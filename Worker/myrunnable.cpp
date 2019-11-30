#include "myrunnable.h"

MyRunnable::MyRunnable(QObject *parent):
    QObject(parent)
{
    DebugInfo::add_object(this);
}

void MyRunnable::abort()
{
    m_abort = true;
}
