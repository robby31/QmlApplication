#include "pythonmodule.h"

PythonModule::PythonModule(QObject *parent):
    QObject(parent)
{
    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(stdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(stdErr(QString)));
}


void PythonModule::stdOut(const QString &message)
{
    std::cout << message.toUtf8().constData();
}

void PythonModule::stdErr(const QString &message)
{
    std::cerr << message.toUtf8().constData();
}

