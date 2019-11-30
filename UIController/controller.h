#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "uiservices.h"
#include "debuginfo.h"

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(int activityProgress READ activityProgress WRITE setActivityProgress NOTIFY activityProgressChanged)
    Q_PROPERTY(QString activity READ activity WRITE setActivity NOTIFY activityChanged)

public:
    explicit Controller(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void abortProcess();

    void setUiServices(UiServices *uiServices);

    bool setActivity(const QString &activity);

    bool busy()                     const;
    int activityProgress()          const;
    QString activity()              const;

    void popMessage(const QString &message, const UiServices::PopupType &type = UiServices::POP_INFO);


private:
    bool setActivityIdle();
    void setActivityProgress(const int &progress);
    void setBusy(const bool &flag = true);


signals:
    void abort();
    void busyChanged();
    void activityChanged();
    void activityProgressChanged();


public slots:
    void processStarted(const QString &activity);
    void activityProgressReceived(const int &progress);
    void processAborted();
    void errorDuringProcess(const QString &errorMessage);
    void processOver(const QString &message = QString());

private slots:
    void uiServicesDestroyed(QObject *obj);

private:
    UiServices *m_uiServices = Q_NULLPTR;
    int m_progressValue = 0;
    QString m_activity;
    bool m_busyIndicator = false;
};

#endif // CONTROLLER_H
