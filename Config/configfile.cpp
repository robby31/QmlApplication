#include "configfile.h"

const QRegExp ConfigFile::tagBegin = QRegExp(R"(^BEGIN\((\w+)\)(.*)$)");
const QRegExp ConfigFile::tagEnd = QRegExp("^END$");
const QRegExp ConfigFile::paramDefinition = QRegExp("^([^=]+)=([^=]*)$");
const QRegExp ConfigFile::paramInQuery = QRegExp("@\\w+@");
const QString ConfigFile::imageTag = "@checkImagePath@";
const QString ConfigFile::checkImagePath = "file:///"+QDir::currentPath()+"/CheckConfigs/Images/";

ConfigFile::ConfigFile(QObject *parent) :
    QFile(parent),
    flag_read(false)
{
}

ConfigFile::ConfigFile(const QFileInfo &fileinfo, QObject *parent) :
    QFile(fileinfo.absoluteFilePath(), parent),
    flag_read(false)
{
}

bool ConfigFile::readConfig()
{
    if (flag_read)
    {
        // no  need to read again the file
        return true;
    }

    data.clear();
    keys.clear();
    QFileInfo fileinfo(fileName());
    last_read = fileinfo.lastModified();
    flag_read = true;

    QString param;
    QByteArray value;
    bool flag_TAG = false;

    if (!open(QFile::ReadOnly))
    {
        // cannot read the file
        return false;
    }

    QTextStream stream(this);
    while (!stream.atEnd())
    {
        QString line = stream.readLine().trimmed();
        line.replace(imageTag, checkImagePath);

        if (!flag_TAG) {
            if (paramDefinition.indexIn(line) != -1) {
                append_data(paramDefinition.cap(1),
                            paramDefinition.cap(2));

            } else if (tagBegin.indexIn(line) != -1) {
                flag_TAG = true;
                param = tagBegin.cap(1);
                if (tagBegin.captureCount() == 2)
                    value.append(tagBegin.cap(2));

            } else if (!line.isEmpty()) {
                qWarning() << "invalid configuration in file" << fileName();
                qWarning() << line;
            }
        } else {
            if (tagEnd.indexIn(line) != -1) {
                append_data(param, value);
                flag_TAG = false;
                value.clear();
            } else {
                value.append(" "+line);
            }
        }
    }

    close();

    return true;
}

bool ConfigFile::append_data(const QString &param, const QString &value) {
    if (param.trimmed().isEmpty()) {
        qWarning() << "invalid parameter in file" << fileName() << " : " << value;
        return false;
     }

    data[param.trimmed()] = value.trimmed();
    keys.append(param.trimmed());

    return true;
}

QList<QString> ConfigFile::getKeys() {
    readConfig();

    return keys;
}

QString ConfigFile::getData(const QString &param) {
    readConfig();

    if (data.contains(param))
        return data[param];

    return QString();
}

QStringList ConfigFile::getDataList(const QString &param)
{
    QStringList res;
    foreach(const QString &elt, getData(param).split(","))
        if (!elt.isNull())
            res.append(elt.trimmed());
    return res;
}

bool ConfigFile::refresh()
{
    QFileInfo fileinfo(fileName());
    if (fileinfo.lastModified() > last_read)
        flag_read = false;
    return readConfig();
}
