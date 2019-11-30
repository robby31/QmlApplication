#include "uiservices.h"

UiServices::UiServices(QObject *parent) :
    QObject(parent)
{
    DebugInfo::add_object(this);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(unPop()));
}

void UiServices::pop(const QString &message, const PopupType &type)
{
    setPopupType(type);
    setPopupText(message);
    setPopupVisible(true);
    m_timer.start(5000);
}

void UiServices::unPop()
{
    setPopupVisible(false);
    m_timer.stop();
}
