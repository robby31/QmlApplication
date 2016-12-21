#ifndef EXCELRANGE_H
#define EXCELRANGE_H

#include <QObject>
#include <QAxObject>
#include <QDebug>

class ExcelRange : public QObject
{
    Q_OBJECT

public:
    explicit ExcelRange(QAxObject *axobj, QObject *parent = 0);

    int rowCount() const { return m_rowCount; }
    int columnCount() const { return m_columnCount; }

    QVariant cellsValue(const int &row, const int &column);

private:
    QAxObject *cells(const int &row, const int &column);

signals:

public slots:

private:
    QAxObject *m_obj;
    QVariant m_values;
    int m_rowCount;
    int m_columnCount;
};

#endif // EXCELRANGE_H
