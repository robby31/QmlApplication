#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#if defined(WEBENGINE_PACKAGE)
#include <QtWebView>
#endif

#if defined(WEBVIEW_PACKAGE)
#include <QtWebView>
#endif

#include "UIController/uiservices.h"
#include "UIController/controller.h"
#include "Worker/worker.h"
#include "Worker/databaseworker.h"

class Application : public QApplication
{
    Q_OBJECT

    Q_PROPERTY(QString databaseDriverName READ databaseDriverName WRITE setdatabaseDiverName NOTIFY databaseDriverNameChanged)
    Q_PROPERTY(QString databaseConnectionName READ databaseConnectionName WRITE setdatabaseConnectionName NOTIFY databaseConnectionNameChanged)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setdatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

public:
    explicit Application(int &argc, char **argv);

    QThread *backendThread() { return &backend; }

    void addToBackend(QObject *obj);
    void addWorker(Controller *controller, Worker *worker);
    void addController(const QString &qmlName, Controller *controller);

    void addImageProvider(const QString &id, QQmlImageProviderBase *provider);

    void loadMainQml(const QUrl &qmlMain);
    void setQmlContextProperty(const QString &name, QObject *obj);
    void setMainWindowTitle(const QString &name);

    QString databaseDriverName() const;
    void setdatabaseDiverName(const QString &driverName);

    QString databaseConnectionName() const;
    void setdatabaseConnectionName(const QString &name);

    void setdatabaseOptions(const QString &options);

    QString databaseName() const;
    void setdatabaseName(const QString &name);

    QSqlDatabase database() const;

    bool isConnected() const;

signals:
    void loadMainQmlSignal(const QUrl &qmlMain);
    void mainQmlLoaded(QObject *obj);

    void databaseDriverNameChanged();
    void databaseConnectionNameChanged();
    void databaseNameChanged();
    void databaseOpened(const QString &databaseName);
    void createDatabaseSignal(const QString &driverName, const QString &connectionName);
    void databaseOptionsSignal(const QString &options);
    void isConnectedChanged();

private slots:
    void objectCreated(QObject *obj, const QUrl &url);
    void createDatabase();

private:
    QThread backend;
    QQmlApplicationEngine qmlEngine;
    UiServices uiServices;
    QUrl mainUrl;

    QString m_databaseDriverName;
    QString m_databaseConnectionName;
    QString m_databaseName;
};

#endif // APPLICATION_H
