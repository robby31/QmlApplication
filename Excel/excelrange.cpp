#include "excelrange.h"

ExcelRange::ExcelRange(QAxObject *axobj, QObject *parent) :
    QObject(parent),
    m_obj(axobj),
    m_rowCount(0),
    m_columnCount(0)
{
    if (m_obj)
    {
        QAxObject* rows = m_obj->querySubObject("Rows");
        if (rows)
        {
            m_rowCount = rows->dynamicCall("Count").toInt();
            delete rows;
        }

        QAxObject* columns = m_obj->querySubObject("Columns");
        if (columns)
        {
            m_columnCount = columns->property("Count").toInt();
            delete columns;
        }
    }
}

QAxObject *ExcelRange::cells(const int &row, const int &column)
{
    if (m_obj)
        return m_obj->querySubObject("Cells(int,int)", row, column);
    else
        return 0;
}

QVariant ExcelRange::cellsValue(const int &row, const int &column)
{
    QAxObject *data = cells(row, column);
    if (data)
    {
        QVariant result = data->property("Value");
        delete data;
        return result;
    }
    else
    {
        return QVariant();
    }
}
