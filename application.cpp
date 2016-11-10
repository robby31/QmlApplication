#include "application.h"

Application::Application(int &argc, char **argv) :
    QApplication(argc, argv),
    backend(),
    qmlEngine(this),
    uiServices(this),
    mainUrl()
{
    #ifdef WEBVIEW_PACKAGE
    QtWebView::initialize();
    #endif

    setQmlContextProperty("_app", this);
    connect(this, SIGNAL(loadMainQmlSignal(QUrl)), &qmlEngine, SLOT(load(QUrl)));
    connect(&qmlEngine, SIGNAL(objectCreated(QObject*,QUrl)), this, SLOT(objectCreated(QObject*,QUrl)));
    connect(&qmlEngine, SIGNAL(quit()), this, SLOT(closeAllWindows()));

    // uiServices
    qmlRegisterType<UiServices>("UiServices", 1, 0, "UiServices");
    setQmlContextProperty("uiServices", &uiServices);

    backend.setObjectName("Backend Application");
    connect(this, SIGNAL(aboutToQuit()), &backend, SLOT(quit()));
    backend.start();
}

void Application::setMainWindowTitle(const QString &name)
{
    qmlEngine.rootObjects()[0]->setProperty("title", name);
}

void Application::addToBackend(QObject *obj)
{
    obj->moveToThread(&backend);
    connect(&backend, SIGNAL(finished()), obj, SLOT(deleteLater()));
}

void Application::addWorker(Controller *controller, Worker *worker)
{
    addToBackend(worker);

    connect(controller, SIGNAL(abort()), worker, SLOT(abortProcess()));
    connect(worker, SIGNAL(processAborted()), controller, SLOT(processAborted()));
    connect(worker, SIGNAL(processOver(QString)), controller, SLOT(processOver(QString)));
    connect(worker, SIGNAL(progress(int)), controller, SLOT(activityProgressReceived(int)));
    connect(worker, SIGNAL(errorDuringProcess(QString)), controller, SLOT(errorDuringProcess(QString)));
}

void Application::addController(const QString &qmlName, Controller *controller)
{
    controller->setUiServices(&uiServices);
    setQmlContextProperty(qmlName, controller);
}

void Application::objectCreated(QObject *obj, const QUrl &url)
{
    if (url == mainUrl)
    {
        if (obj)
        {
            emit mainQmlLoaded(obj);
        }
        else
        {
            // main window not loaded
            backend.quit();
            deleteLater();
        }
    }
}

void Application::loadMainQml(const QUrl &qmlMain)
{
    mainUrl = qmlMain;
    emit loadMainQmlSignal(qmlMain);
}

void Application::setQmlContextProperty(const QString &name, QObject *obj)
{
    qmlEngine.rootContext()->setContextProperty(name, obj);
}
