#include "application.h"

Application::Application(int &argc, char **argv) :
    QApplication(argc, argv),
    backend(),
    qmlEngine(this),
    uiServices(this),
    mainUrl()
{
#if defined(WEBENGINE_PACKAGE)
    QtWebEngine::initialize();
#endif

#if defined(WEBVIEW_PACKAGE)
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

    connect(this, SIGNAL(databaseDriverNameChanged()), this, SLOT(createDatabase()));
    connect(this, SIGNAL(databaseConnectionNameChanged()), this, SLOT(createDatabase()));

    qRegisterMetaType<QSqlDatabase>("QSqlDatabase");

    // worker to initialize database in backend Thread
    DatabaseWorker *worker = new DatabaseWorker();
    addToBackend(worker);
    connect(this, SIGNAL(createDatabaseSignal(QString,QString)), worker, SLOT(createDatabase(QString,QString)));
    connect(this, SIGNAL(databaseOptionsSignal(QString)), worker, SLOT(databaseOptions(QString)));
    connect(this, SIGNAL(databaseOpened(QUrl)), worker, SLOT(databaseOpened(QUrl)));
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

QString Application::databaseDriverName() const
{
    return m_databaseDriverName;
}

void Application::setdatabaseDiverName(const QString &driverName)
{
    if (!m_databaseDriverName.isEmpty())
        qWarning() << "current database conncection will be lost.";

    m_databaseDriverName = driverName;
    emit databaseDriverNameChanged();
}

QString Application::databaseConnectionName() const
{
    return m_databaseConnectionName;
}

void Application::setdatabaseConnectionName(const QString &name)
{
    if (!m_databaseConnectionName.isEmpty())
        qWarning() << "current database conncection will be lost.";

    m_databaseConnectionName = name;
    emit databaseConnectionNameChanged();
}

void Application::setdatabaseOptions(const QString &options)
{
    QSqlDatabase db = database();

    if (db.isValid())
    {
        db.setConnectOptions(options);
        emit databaseOptionsSignal(options);
    }
    else
    {
        qCritical() << this << "invalid database, unable to set options" << options;
    }
}

QUrl Application::databasePathName() const
{
    return m_databasePathName;
}

void Application::setdatabasePathName(const QUrl path)
{
    QSqlDatabase db = GET_DATABASE(databaseConnectionName());

    if (!db.isValid())
    {
        qCritical() << "invalid database" << databaseDriverName() << databaseConnectionName();
    }
    else
    {
        if (path.isLocalFile())
        {

            if (db.isOpen())
                db.close();

            db.setDatabaseName(path.toLocalFile());

            if (!db.open())
            {
                qCritical() << "unable to open database" << db.lastError().text();
            }
            else
            {
                m_databasePathName = path;
                emit databasePathNameChanged();
                emit databaseOpened(path);
            }
        }
        else
        {
            qCritical() << "invalid database pathname" << path;
        }
    }
}

void Application::createDatabase()
{
    if (!databaseDriverName().isEmpty() && !databaseConnectionName().isEmpty())
    {
        CREATE_DATABASE(databaseDriverName(), databaseConnectionName());
        emit createDatabaseSignal(databaseDriverName(), databaseConnectionName());
    }
}

QSqlDatabase Application::database() const
{
    return GET_DATABASE(databaseConnectionName());
}
