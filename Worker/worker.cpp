#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent),
    m_running(false)
{
    connect(this, SIGNAL(processStarted()),             this, SLOT(_processStarted()));
    connect(this, SIGNAL(progress(int)),                this, SLOT(_progress(int)));
    connect(this, SIGNAL(errorDuringProcess(QString)),  this, SLOT(_errorDuringProcess(QString)));
    connect(this, SIGNAL(processAborted()),             this, SLOT(_processAborted()));
    connect(this, SIGNAL(processOver(QString)),         this, SLOT(_processOver(QString)));
}

bool Worker::processWasAborted()
{
    QCoreApplication::processEvents();
    if (m_running)
    {
        return false;
    }
    else
    {
        emit processAborted();
        return true;
    }
}

void Worker::_processStarted()
{
    if (m_running)
        qWarning() << "process is already started in worker" << this;
    else
        m_running = true;
}

void Worker::_progress(const int &progress)
{
    Q_UNUSED(progress)
    if (!m_running)
        qWarning() << "progress received but process is not started in worker" << this;
}

void Worker::_errorDuringProcess(const QString &errorMessage)
{
    Q_UNUSED(errorMessage)
    if (!m_running)
        qWarning() << "error received but process is not started in worker" << this;
    else
        m_running = false;
}

void Worker::_processAborted()
{
    if (!m_running)
        qWarning() << "process aborted but never started in worker" << this;
    else
        m_running = false;
}

void Worker::_processOver(const QString &message)
{
    Q_UNUSED(message)
    if (!m_running)
        qWarning() << "process finished but never started in worker" << this;
    else
        m_running = false;
}
