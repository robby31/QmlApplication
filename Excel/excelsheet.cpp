#include "excelsheet.h"

ExcelSheet::ExcelSheet(QAxObject *axsheet, QObject *parent) :
    QObject(parent),
    m_sheet(axsheet),
    m_rowCount(0),
    m_columnCount(0)
{
    if (m_sheet)
    {
        QAxObject* usedrange = m_sheet->querySubObject("UsedRange");

        if (usedrange)
        {
            QAxObject* rows = usedrange->querySubObject("Rows");
            if (rows)
                m_rowCount = rows->dynamicCall("Count").toInt();

            QAxObject* columns = usedrange->querySubObject("Columns");
            if (columns)
                m_columnCount = columns->property("Count").toInt();

            delete usedrange;
        }
    }
}

QAxObject *ExcelSheet::cells(const int &row, const int &column)
{
    if (m_sheet)
        return m_sheet->querySubObject("Cells(int,int)", row, column);
    else
        return 0;
}

QVariant ExcelSheet::cellsValue(const int &row, const int &column)
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

ExcelRange *ExcelSheet::range(const QString &name)
{
    if (m_sheet)
    {
        QAxObject *obj = m_sheet->querySubObject("Range(string)", name);
        if (obj)
        {
            ExcelRange *range = new ExcelRange(obj, this);
            return range;
        }
    }

    return 0;
}

QString ExcelSheet::name() const
{
    if (m_sheet)
        return m_sheet->property("Name").toString();
    else
        return QString();

}
