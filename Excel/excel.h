#ifndef EXCEL_H
#define EXCEL_H

#include <QObject>
#include <QDebug>
#include <QUrl>
#include <QAxObject>
#include "excelsheet.h"

class Excel : public QObject
{
    Q_OBJECT

public:
    explicit Excel(QObject *parent = 0);
    virtual ~Excel();

    bool newWorkbook();
    bool open(QUrl pathname);
    bool saveAs(QUrl pathname);
    void close();
    ExcelSheet *getSheet(const QString &name);
    ExcelSheet *getSheet(const int &index);

signals:

public slots:

private:
     QAxObject *m_excel;
     QAxObject *m_workbook;
     QAxObject *m_sheets;
};

#endif // EXCEL_H
