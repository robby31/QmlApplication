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
    Q_PROPERTY(QUrl databasePathName READ databasePathName WRITE setdatabasePathName NOTIFY databasePathNameChanged)

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

    QUrl databasePathName() const;
    void setdatabasePathName(const QUrl path);

    QSqlDatabase database() const;

signals:
    void loadMainQmlSignal(const QUrl &qmlMain);
    void mainQmlLoaded(QObject *obj);

    void databaseDriverNameChanged();
    void databaseConnectionNameChanged();
    void databasePathNameChanged();
    void databaseOpened(const QUrl &path);
    void createDatabaseSignal(const QString &driverName, const QString &connectionName);
    void databaseOptionsSignal(const QString &options);

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
    QUrl m_databasePathName;
};

#endif // APPLICATION_H
