#include "checkedsqllistmodel.h"

CheckedSqlListModel::CheckedSqlListModel(QObject *parent) :
    BaseSqlListModel(parent)
{
    addCustomRole("checked");
    addCustomRole("name");

    connect(this, SIGNAL(tablenameChanged()), this, SLOT(init_query()));
    connect(this, SIGNAL(queryDataChanged()), this, SLOT(init_query()));
    connect(this, SIGNAL(parameterChanged()), this, SLOT(init_query()));
}

void CheckedSqlListModel::init_query()
{
    if (!m_queryData.isEmpty() && !m_parameter.isEmpty())
        setQuery(QString("SELECT DISTINCT %2 from (%1) AS checked_query").arg(m_queryData, m_parameter));

    if (!lastError().trimmed().isEmpty())
    {
        if (!tablename().isEmpty() && !m_parameter.isEmpty())
            setQuery(QString("SELECT DISTINCT %2 from %1").arg(tablename(), m_parameter));
        else
            setQuery(QString());
    }
}

QVariant CheckedSqlListModel::data(const QModelIndex &index, int role) const
{
    QString strRole = QVariant::fromValue(roleNames()[role]).toString();

    if (strRole == "checked")
    {
        QString name = BaseSqlListModel::data(index, Qt::UserRole).toString();

        if (m_allChecked)
            return !m_filteredName.contains(name);

        return m_filteredName.contains(name);
    }

    if (strRole == "name")
        return BaseSqlListModel::data(index, Qt::UserRole);

    return BaseSqlListModel::data(index, role);
}

void CheckedSqlListModel::setQueryData(const QString &query)
{
    m_queryData = query;
    emit queryDataChanged();
}

bool CheckedSqlListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString strRole = QVariant::fromValue(roleNames()[role]).toString();

    if (strRole == "checked")
    {
        QString name = BaseSqlListModel::data(index, Qt::UserRole).toString();

        if (m_allChecked)
        {
            if (!value.toBool())
            {
                m_filteredName << name;
                emit dataChanged(index, index);
                return true;
            }

            if (m_filteredName.contains(name))
            {
                m_filteredName.removeOne(name);
                emit dataChanged(index, index);
                return true;
            }
        }
        else
        {
            if (value.toBool())
            {
                m_filteredName.append(name);
                emit dataChanged(index, index);
                return true;
            }

            if (m_filteredName.contains(name))
            {
                m_filteredName.removeOne(name);
                emit dataChanged(index, index);
                return true;
            }
        }
    }

    return false;
}

void CheckedSqlListModel::selectAll()
{
    setAllChecked(true);
    m_filteredName.clear();

    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 1));
}

void CheckedSqlListModel::unSelectAll()
{
    setAllChecked(false);
    m_filteredName.clear();

    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 1));
}

QString CheckedSqlListModel::checkedFilterCmd()
{
    QString strRole = roleNames()[Qt::UserRole];

    bool filteredStringNull = false;
    QStringList filteredString;
    foreach (const QString &name, m_filteredName)
    {
        if (name.isNull())
            filteredStringNull = true;
        else if (!name.contains("'"))
            filteredString << QString("'%1'").arg(name);
        else
            filteredString << QString("\"%1\"").arg(name);
    }

    if (m_allChecked  && (!filteredString.isEmpty() || filteredStringNull))
    {
        if (!filteredString.isEmpty())
        {
            if (filteredStringNull)
                return QString("%1 not in (%2)").arg(strRole, filteredString.join(','));

            return QString("(%1 is null or %1 not in (%2))").arg(strRole, filteredString.join(','));
        }

        if (filteredStringNull)
            return QString("%1 is not null").arg(strRole);
    }
    else if (!m_allChecked && (!filteredString.isEmpty() || filteredStringNull))
    {
        if (!filteredString.isEmpty())
        {
            if (filteredStringNull)
                return QString("(%1 is null or %1 in (%2))").arg(strRole, filteredString.join(','));

            return QString("%1 in (%2)").arg(strRole, filteredString.join(','));
        }

        if (filteredStringNull)
            return QString("%1 is null").arg(strRole);
    }

    return QString();
}

void CheckedSqlListModel::setTextFilter(const QString &text)
{
    m_textFilter = text;
    emit textFilterChanged();

    if (text != "")
        setFilter(QString("%1 LIKE '%%%2%%'").arg(m_parameter, text));
    else
        setFilter("");
}

void CheckedSqlListModel::setParameter(const QString &value)
{
    m_parameter = value;
    emit parameterChanged();

    setOrderBy(m_parameter);
}
