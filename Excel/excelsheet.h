#ifndef EXCELSHEET_H
#define EXCELSHEET_H

#include <QObject>
#include <QAxObject>
#include <QDebug>
#include "excelrange.h"

class ExcelSheet : public QObject
{
    Q_OBJECT

public:
    explicit ExcelSheet(QAxObject *axsheet, QObject *parent = 0);

    int rowCount() const { return m_rowCount; }
    int columnCount() const { return m_columnCount; }

    QString name() const;

    QVariant cellsValue(const int &row, const int &column);
    bool writeValue(const int &row, const int &column, const QVariant &value);

    ExcelRange *range(const QString &name);

private:
    QAxObject *cells(const int &row, const int &column);

signals:

public slots:

private:
    QAxObject *m_sheet;
    QVariant m_values;
    int m_rowCount;
    int m_columnCount;
};

#endif // EXCELSHEET_H
