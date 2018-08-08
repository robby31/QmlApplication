#include "myrunnable.h"

MyRunnable::MyRunnable():
    m_abort(false)
{

}

void MyRunnable::abort()
{
    m_abort = true;
}
