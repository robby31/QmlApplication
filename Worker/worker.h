#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QMutex>
#include <QWaitCondition>
#include "debuginfo.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = Q_NULLPTR);

    bool isRunning() const { return m_running; }

protected:
    bool processWasAborted();

signals:
    void processStarted();
    void progress(const int &progress);
    void errorDuringProcess(const QString &errorMessage);
    void processAborted();
    void processOver(const QString &message = QString());

public slots:
    void abortProcess() { emit processAborted(); }

private slots:
    void _processStarted();
    void _progress(const int &progress);
    void _errorDuringProcess(const QString &errorMessage);
    void _processAborted();
    void _processOver(const QString &message = QString());

private:
    bool m_running = false;
};

#endif // WORKER_H
