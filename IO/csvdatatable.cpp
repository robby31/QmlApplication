#include "csvdatatable.h"

CsvDataTable::CsvDataTable(QObject *parent):
    QObject(parent),
    m_titleLine()
{
}

void CsvDataTable::setTitle(const QStringList &titles)
{
    l_title = titles;
    m_titleLine = titles.join(';');
}

void CsvDataTable::appendData(const QStringList &data, QHash<QString, QVariant> *filter, QHash<QString, CheckedListModel *> *columnsDataModel)
{
    if (data.count() != l_title.count())
        qWarning() << "WARNING, columns number differ compared to titles" << data.count() << l_title.count();

    if (!dataFiltered(data, filter, columnsDataModel))
    {
        l_data.append(data);
        l_lines.append(data.join(';'));
    }
}

bool CsvDataTable::dataFiltered(const QStringList &data, QHash<QString, QVariant> *filter, QHash<QString, CheckedListModel *> *columnsDataModel)
{
    if (filter && !filter->isEmpty())
    {
        for (auto param = filter->constBegin(); param != filter->constEnd(); ++param)
        {
            int index = l_title.indexOf(param.key());
            if (index>=0 && index<data.count())
                if (data.at(index) != param.value())
                    return true;
        }
    }

    if (columnsDataModel && !columnsDataModel->isEmpty())
    {
        for (auto title = columnsDataModel->constBegin(); title !=  columnsDataModel->constEnd(); ++title)
        {
            int index = l_title.indexOf(title.key());
            if (index>=0 && index<data.count())
            {
                const QString& value = data.at(index);

                CheckedListModel *model = title.value();
                if (model)
                {
                    CheckedItem *item = model->findByRole(value, CheckedItem::NameRole);
                    if (item && !item->data(CheckedItem::CheckedRole).toBool())
                        return true;
                }
            }
        }
    }

    return false;
}

bool CsvDataTable::loadFile(const QString &pathname, QHash<QString, QVariant> *filter, QHash<QString, CheckedListModel*> *columnsDataModel)
{
    l_title.clear();
    l_data.clear();
    m_titleLine.clear();
    l_lines.clear();

    QFile fd(pathname);

    if (!fd.open(QIODevice::ReadOnly))
    {
        qWarning() << "Unable to read" << fd.fileName();
        return false;
    }

    QTextStream textFile(&fd);

    // read first line: titles
    if (!textFile.atEnd()) {
        QString line(textFile.readLine());
        m_titleLine = line;
        foreach (const QString &data, line.split(";"))
            l_title << data;
    }

    // read data
    while (!textFile.atEnd()) {
        QString line(textFile.readLine());

        QStringList tmp;
        if (line.contains('"'))
        {
            int offset = 0;
            QRegularExpression patternBegin("(^|;)\"");
            QRegularExpression patternEnd("\"(;|$)");

            QRegularExpressionMatch matchBegin = patternBegin.match(line, offset);
            while (matchBegin.hasMatch())
            {
                QString text = line.left(matchBegin.capturedStart()).right(matchBegin.capturedStart()-offset);
                //                    qWarning() << "BEGIN" << matchBegin.capturedStart() << matchBegin.capturedEnd() << text;
                if (!text.isEmpty())
                {
                    if (offset>0 && text.startsWith(';'))
                        text = text.right(text.size()-1);
                    tmp << text.split(';');
                }

                offset = matchBegin.capturedEnd();

                QRegularExpressionMatch matchEnd = patternEnd.match(line, offset);
                while (!matchEnd.hasMatch() && !textFile.atEnd())
                {
                    line += textFile.readLine();
                    matchEnd = patternEnd.match(line, offset);
                }

                if (matchEnd.hasMatch())
                {
                    QString text = line.left(matchEnd.capturedStart()).right(matchEnd.capturedStart()-offset);
                    //                        qWarning() << "END" << matchEnd.capturedStart() << matchEnd.capturedEnd() << text;
                    tmp << text;
                    offset = matchEnd.capturedEnd()-1;
                }

                matchBegin = patternBegin.match(line, offset);
                //                    qWarning() << "NEW LOOP" << line.left(offset);
            }

            QString text = line.right(line.size()-offset);
            //                qWarning() << "BREAK" << text;
            if (!text.isEmpty())
            {
                if (text.startsWith(';'))
                    text = text.right(text.size()-1);
                tmp << text.split(';');
            }
        }
        else
        {
            tmp = line.split(";");
        }

        if (tmp.count() != l_title.count())
        {
            qWarning() << "WARNING, columns number differ compared to titles" << tmp.count() << l_title.count();
            qWarning() << line;
            qWarning() << tmp;
        }



        if (!dataFiltered(tmp, filter, columnsDataModel))
        {
            l_data.append(tmp);
            l_lines << line;
        }

        tmp.clear();
    }

    // close file
    fd.close();

    return true;
}

bool CsvDataTable::loadFile(CsvDataTable *input, QHash<QString, QVariant> *filter, QHash<QString, CheckedListModel *> *columnsDataModel)
{
    l_title.clear();
    l_data.clear();
    m_titleLine.clear();
    l_lines.clear();

    setTitle(input->title());

    foreach (const QStringList &data, input->data())
        appendData(data, filter, columnsDataModel);

    return true;
}

QString CsvDataTable::getValue(const int &index, const int &column) const
{
    if (index>=0 && index <l_data.size()) {
        QStringList data = l_data.at(index);
        if (column>=0 && column<data.size())
            return data.at(column);
    }
    return QString();
}

QString CsvDataTable::getValue(const int &index, const QString &param) const
{
    if (l_title.contains(param))
        return getValue(index, l_title.indexOf(param));

    return QString();
}

int CsvDataTable::searchIndex(const QString &param, const QString &value) const
{
    if (!l_title.contains(param))
        return -1;

    int columnIndex = l_title.indexOf(param);

    int res = -1;
    int index = 0;
    foreach (const QStringList &line, l_data) {
        if (line.at(columnIndex)==value) {
            if (res==-1)
                res = index;
            else
                return -1;  // several line matches the value of searched parameter
        }
        ++index;
    }

    return res;
}
