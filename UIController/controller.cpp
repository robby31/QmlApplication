#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_activity("IDLE")      // default value when controller is ready
{
    DebugInfo::add_object(this);
}

void Controller::abortProcess()
{
    emit abort();
}

void Controller::setUiServices(UiServices *uiServices)
{
    m_uiServices = uiServices;
    connect(uiServices, &UiServices::destroyed, this, &Controller::uiServicesDestroyed);
}

bool Controller::setActivity(const QString &activity)
{
    if (activity == "IDLE")
    {
        m_activity = activity;
        emit(activityChanged());
        setActivityProgress(-1);
        setBusy(false);
        return true;

    }

    if (m_activity != "IDLE")
    {
        popMessage(QString("%1 in progress, operation %2 not started").arg(m_activity, activity),
                   UiServices::POP_WARNING);
        return false;
    }

    m_activity = activity;
    emit(activityChanged());
    setActivityProgress(-1);
    return true;
}

bool Controller::busy() const
{
    return m_busyIndicator;
}

int Controller::activityProgress() const
{
    return m_progressValue;
}

QString Controller::activity() const
{
    return m_activity;
}

void Controller::popMessage(const QString &message, const UiServices::PopupType &type)
{
    if (m_uiServices)
        m_uiServices->pop(message, type);
}

bool Controller::setActivityIdle()
{
    return setActivity("IDLE");
}

void Controller::setActivityProgress(const int &progress)
{
    m_progressValue = progress;
    emit activityProgressChanged();
}

void Controller::setBusy(const bool &flag)
{
    m_busyIndicator = flag;
    emit busyChanged();
}

void Controller::processStarted(const QString &activity)
{
    setActivity(activity);
}

void Controller::activityProgressReceived(const int &progress)
{
    setActivityProgress(progress);
    if (!m_busyIndicator)
        setBusy();
}

void Controller::errorDuringProcess(const QString &errorMessage)
{
    popMessage("ERROR: "+errorMessage, UiServices::POP_ERROR);
    setActivityIdle();
}

void Controller::processAborted()
{
    if (m_activity != "IDLE")
    {
        popMessage(QString("%1 aborted").arg(m_activity), UiServices::POP_WARNING);
        setActivityIdle();
    }
}

void Controller::processOver(const QString &message)
{
    if (!message.isEmpty())
        popMessage(message);
    else if (m_activity != "IDLE")
        popMessage(QString("%1 done.").arg(m_activity));

    setActivityIdle();
}

void Controller::uiServicesDestroyed(QObject *obj)
{
    if (m_uiServices == obj)
        m_uiServices = Q_NULLPTR;
}
