#ifndef CSVDATATABLE_H
#define CSVDATATABLE_H

#include <QObject>
#include <QFile>
#include <QRegularExpression>
#include <QDebug>
#include "Models/checkedlistmodel.h"
#include "Models/checkeditem.h"

class CsvDataTable : public QObject
{
    Q_OBJECT

public:
    explicit CsvDataTable(QObject *parent = 0);

    bool loadFile(const QString &pathname, QHash<QString, QVariant> *filter=0, QHash<QString, CheckedListModel*> *columnsDataModel=0);
    bool loadFile(CsvDataTable *input, QHash<QString, QVariant> *filter=0, QHash<QString, CheckedListModel*> *columnsDataModel=0);

signals:

public slots:
    QStringList title() const { return l_title; }
    int columnCount() const { return l_title.size(); }
    int rowCount() const { return l_data.size(); }
    QList<QStringList> data() const { return l_data; }
    QString titleLine() const { return m_titleLine; }
    QString getLine(const int &lineNumber) const { if (lineNumber<l_lines.count()) return l_lines.at(lineNumber);
                                                   else return QString(); }

    bool isTitleValid(const QString &title) { return l_title.contains(title); }

    void setTitle(const QStringList &titles);
    void appendData(const QStringList &data, QHash<QString, QVariant> *filter=0, QHash<QString, CheckedListModel*> *columnsDataModel=0);

    QString getValue(const int &index, const QString &param) const;
    QString getValue(const int &index, const int &column) const;
    int searchIndex(const QString &param, const QString &value) const;

private:
    bool dataFiltered(QStringList data, QHash<QString, QVariant> *filter=0, QHash<QString, CheckedListModel*> *columnsDataModel=0);

private:
    QStringList l_title;
    QList<QStringList> l_data;
    QString m_titleLine;
    QStringList l_lines;
};

#endif // CSVDATATABLE_H
