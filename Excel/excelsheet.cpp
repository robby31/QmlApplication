#include "excelsheet.h"

ExcelSheet::ExcelSheet(QAxObject *axsheet, QObject *parent) :
    QObject(parent),
    m_sheet(axsheet),
    m_values(),
    m_rowCount(0),
    m_columnCount(0)
{
    if (m_sheet)
    {
        QAxObject* usedrange = m_sheet->querySubObject("UsedRange");

        if (usedrange)
        {
            m_values = usedrange->property("Value");

            m_rowCount = m_values.toList().size();

            if (m_rowCount > 0)
                m_columnCount = m_values.toList().at(0).toList().size();

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
    if (row >=0 && row < m_rowCount && column>=0 && column < m_columnCount)
    {
        return m_values.toList().at(row).toList().at(column);
    }
    else
    {
        qWarning() << "invalid row or column" << row << m_rowCount << " / " << column << m_columnCount;
        return QVariant();
    }
}

bool ExcelSheet::writeValue(const int &row, const int &column, const QVariant &value)
{
    QAxObject *data = cells(row, column);
    if (data)
    {
        data->dynamicCall("SetValue(const QVariant&)", value);
        delete data;
        return true;
    }
    else
    {
        return false;
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
