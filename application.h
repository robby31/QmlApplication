#ifndef APPLICATION_H
#define APPLICATION_H

#include "libqmlapplication_global.h"
#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include "UIController/uiservices.h"
#include "UIController/controller.h"
#include "Worker/worker.h"

class QMLAPPLICATIONSHARED_EXPORT Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);

    void addToBackend(QObject *obj);
    void addWorker(Controller *controller, Worker *worker);
    void addController(const QString &qmlName, Controller *controller);

    void loadMainQml(const QUrl &qmlMain)                         { mainUrl = qmlMain;     emit loadMainQmlSignal(qmlMain); }
    void setQmlContextProperty(const QString &name, QObject *obj) { qmlEngine.rootContext()->setContextProperty(name, obj); }
    void setMainWindowTitle(const QString &name);

signals:
    void loadMainQmlSignal(const QUrl &qmlMain);
    void mainQmlLoaded(QObject *obj);

private slots:
    void objectCreated(QObject *obj, const QUrl &url);

private:
    QThread backend;
    QQmlApplicationEngine qmlEngine;
    UiServices uiServices;
    QUrl mainUrl;
};

#endif // APPLICATION_H
