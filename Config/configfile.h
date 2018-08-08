#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QTextStream>

#include "Models/listmodel.h"

class ConfigFile : public QFile
{
    Q_OBJECT

public:
    explicit ConfigFile(QObject *parent = 0);
    ConfigFile(const QFileInfo &fileinfo, QObject *parent = 0);

    QList<QString> getKeys();
    QString getData(const QString &param);
    QStringList getDataList(const QString &param);

    bool refresh();

    QString name() { return QFileInfo(fileName()).baseName(); }

private:
    bool readConfig();
    bool append_data(const QString &param, const QString &value);

signals:

public slots:

private:
    QDateTime last_read;
    bool flag_read;
    QHash<QString, QString> data;
    QStringList keys;

    static const QRegExp tagBegin;
    static const QRegExp tagEnd;
    static const QRegExp paramDefinition;
    static const QRegExp paramInQuery;

    static const QString imageTag;
    static const QString checkImagePath;
};

#endif // CONFIGFILE_H
