#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "libqmlapplication_global.h"
#include <QObject>

#include "uiservices.h"


class QMLAPPLICATIONSHARED_EXPORT Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(int activityProgress READ activityProgress WRITE setActivityProgress NOTIFY activityProgressChanged)
    Q_PROPERTY(QString activity READ activity WRITE setActivity NOTIFY activityChanged)

public:
    explicit Controller(QObject *parent = 0);

    Q_INVOKABLE void abortProcess() {emit abort();}

    void setUiServices(UiServices *uiServices) { m_uiServices = uiServices; }

    bool setActivity(const QString &activity);

    bool busy()                     const {return m_busyIndicator;}
    int activityProgress()          const {return m_progressValue;}
    QString activity()              const {return m_activity;}

    void popMessage(const QString &message, const UiServices::PopupType &type = UiServices::POP_INFO) { if (m_uiServices) m_uiServices->pop(message, type); }


private:
    bool setActivityIdle() { return setActivity("IDLE"); }
    void setActivityProgress(const int &progress) {m_progressValue = progress; emit activityProgressChanged();}
    void setBusy(const bool &flag = true) { m_busyIndicator = flag; emit busyChanged(); }


signals:
    void abort();
    void busyChanged();
    void activityChanged();
    void activityProgressChanged();


public slots:
    void processStarted(const QString &activity)                {setActivity(activity);}
    void activityProgressReceived(const int &progress)          {setActivityProgress(progress); if (!m_busyIndicator) setBusy();}
    void processAborted();
    void errorDuringProcess(const QString &errorMessage)        {popMessage("ERROR: "+errorMessage, UiServices::POP_ERROR);                  setActivityIdle();}
    void processOver(const QString &message = 0);


private:
    UiServices *m_uiServices;
    int m_progressValue;
    QString m_activity;
    bool m_busyIndicator;
};

#endif // CONTROLLER_H
