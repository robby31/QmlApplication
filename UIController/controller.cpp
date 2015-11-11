#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_uiServices(0),
    m_progressValue(0),
    m_activity("IDLE"),      // default value when controller is ready
    m_busyIndicator(false)
{
}

bool Controller::setActivity(const QString &activity)
{
    if (activity == "IDLE") {
        m_activity = activity;
        emit(activityChanged());
        setActivityProgress(-1);
        setBusy(false);
        return true;

    } else {

        if (m_activity != "IDLE") {
            popMessage(QString("%1 in progress, operation %2 not started").arg(m_activity).arg(activity),
                       UiServices::POP_WARNING);
            return false;
        } else {
            m_activity = activity;
            emit(activityChanged());
            setActivityProgress(-1);
            return true;
        }
    }
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
