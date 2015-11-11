#ifndef WORKER_H
#define WORKER_H

#include "libqmlapplication_global.h"
#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QMutex>
#include <QWaitCondition>

class QMLAPPLICATIONSHARED_EXPORT Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    virtual ~Worker() {}

    bool isRunning() const { return m_running; }

protected:
    bool processWasAborted();

signals:
    void processStarted();
    void progress(const int &progress);
    void errorDuringProcess(const QString &errorMessage);
    void processAborted();
    void processOver(const QString &message = 0);

public slots:
    void abortProcess() { emit processAborted(); }

private slots:
    void _processStarted();
    void _progress(const int &progress);
    void _errorDuringProcess(const QString &errorMessage);
    void _processAborted();
    void _processOver(const QString &message = 0);

private:
    bool m_running;
};

#endif // WORKER_H
