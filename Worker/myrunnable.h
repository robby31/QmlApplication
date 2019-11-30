#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "debuginfo.h"

class MyRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit MyRunnable(QObject *parent = Q_NULLPTR);

    bool isAborted() const { return m_abort; }

private slots:
    void abort();

private:
    bool m_abort = false;
};

#endif // MYRUNNABLE_H
