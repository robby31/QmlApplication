#include "myrunnable.h"

MyRunnable::MyRunnable():
    QObject(),
    QRunnable(),
    m_abort(false)
{

}

void MyRunnable::abort()
{
    m_abort = true;
}
