#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include "libqmlapplication_global.h"

#include <QObject>
#include <QRunnable>

class QMLAPPLICATIONSHARED_EXPORT MyRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit MyRunnable();

    bool isAborted() const { return m_abort; }

signals:

private slots:
    void abort();

private:
    bool m_abort;
};

#endif // MYRUNNABLE_H
