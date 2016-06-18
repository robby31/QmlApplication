#include "excel.h"

Excel::Excel(QObject *parent) :
    QObject(parent),
    m_excel(0),
    m_workbook(0),
    m_sheets(0)
{
    m_excel = new QAxObject("Excel.Application", this);
    m_excel->setProperty("Visible", false);
}

Excel::~Excel()
{
    close();
}

bool Excel::open(QUrl pathname)
{
    QAxObject *workbooks = m_excel->querySubObject("Workbooks");

    if (workbooks)
    {
        m_workbook = workbooks->querySubObject("Open(const QString&, Boolean, Boolean)", pathname, false, false);
        if (m_workbook)
        {
            m_sheets = m_workbook->querySubObject("Worksheets");
            if (m_sheets)
                return true;
        }
    }

    return false;
}

bool Excel::saveAs(QUrl pathname)
{
    if (m_workbook)
    {
        m_workbook->querySubObject("SaveAs (const QString&)", pathname);
        return true;
    }

    return false;
}

void Excel::close()
{
    if (m_workbook)
        m_workbook->dynamicCall("Close(Boolean)", false);
}

ExcelSheet *Excel::getSheet(const QString &name)
{
    if (m_sheets)
    {
        int count = m_sheets->property("Count").toInt();
        if (count > 0)
        {
            for (int index=1;index<=count;++index)
            {
                QAxObject *sheet = m_sheets->querySubObject("Item( int )", index);
                if (sheet)
                {
                    if (sheet->property("Name").toString() == name)
                    {
                        ExcelSheet *result = new ExcelSheet(sheet, this);
                        return result;
                    }
                }
            }
        }
    }

    return 0;
}

ExcelSheet *Excel::getSheet(const int &index)
{
    if (index > 0 && m_sheets)
    {
        int count = m_sheets->property("Count").toInt();
        if (index <= count)
        {
            QAxObject *sheet = m_sheets->querySubObject("Item( int )", index);
            if (sheet)
            {
                ExcelSheet *result = new ExcelSheet(sheet, this);
                return result;
            }
        }
    }

    return 0;
}
