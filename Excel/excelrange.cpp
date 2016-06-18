#include "excelrange.h"

ExcelRange::ExcelRange(QAxObject *axobj, QObject *parent) :
    QObject(parent),
    m_obj(axobj),
    m_values(),
    m_rowCount(0),
    m_columnCount(0)
{
    if (m_obj)
    {
        m_values = m_obj->property("Value");

        m_rowCount = m_values.toList().size();

        if (m_rowCount > 0)
            m_columnCount = m_values.toList().at(0).toList().size();
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
