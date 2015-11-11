#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "libqmlapplication_global.h"

#include <iostream>
#include <PythonQt.h>

class QMLAPPLICATIONSHARED_EXPORT PythonModule : public QObject
{
    Q_OBJECT

public:
    explicit PythonModule(QObject *parent = 0);

    PythonQtObjectPtr mainContext()   const { return PythonQt::self()->getMainModule();      }
    PythonQtObjectPtr uniqueContext() const { return PythonQt::self()->createUniqueModule(); }

signals:

private slots:
    void stdOut(const QString &message);
    void stdErr(const QString &message);

};

#endif // PYTHONMODULE_H
