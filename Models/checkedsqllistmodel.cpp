#include "checkedsqllistmodel.h"

CheckedSqlListModel::CheckedSqlListModel(QObject *parent) :
    BaseSqlListModel(parent),
    m_parameter(),
    m_allChecked(true),
    m_filteredName()
{
    addCustomRole("checked");
    addCustomRole("name");

    connect(this, SIGNAL(tablenameChanged()), this, SLOT(init_query()));
    connect(this, SIGNAL(parameterChanged()), this, SLOT(init_query()));
}

void CheckedSqlListModel::init_query()
{
    if (!tablename().isEmpty() && !m_parameter.isEmpty())
        setQuery(QString("SELECT DISTINCT %2 from %1 ORDER BY %2").arg(tablename()).arg(m_parameter));
    else
        setQuery(QString());
}

QVariant CheckedSqlListModel::data(const QModelIndex &index, int role) const
{
    QString strRole = QVariant::fromValue(roleNames()[role]).toString();

    if (strRole == "checked")
    {
        QString name = BaseSqlListModel::data(index, Qt::UserRole).toString();

        if (m_allChecked)
            return !m_filteredName.contains(name);
        else
            return m_filteredName.contains(name);
    }
    else if (strRole == "name")
        return BaseSqlListModel::data(index, Qt::UserRole);
    else
        return BaseSqlListModel::data(index, role);
}

bool CheckedSqlListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QVector<int> roles;
    roles << Qt::UserRole+1;

    QString strRole = QVariant::fromValue(roleNames()[role]).toString();

    if (strRole == "checked")
    {
        QString name = BaseSqlListModel::data(index, Qt::UserRole).toString();

        if (m_allChecked)
        {
            if (!value.toBool())
            {
                m_filteredName << name;
                emit dataChanged(index, index, roles);
                return true;
            }
            else
            {
                if (m_filteredName.contains(name))
                {
                    m_filteredName.removeOne(name);
                    emit dataChanged(index, index, roles);
                    return true;
                }
            }
        }
        else
        {
            if (value.toBool())
            {
                m_filteredName.append(name);
                emit dataChanged(index, index, roles);
                return true;
            }
            else
            {
                if (m_filteredName.contains(name))
                {
                    m_filteredName.removeOne(name);
                    emit dataChanged(index, index, roles);
                    return true;
                }
            }
        }
    }

    return false;
}

void CheckedSqlListModel::selectAll()
{
    setAllChecked(true);
    m_filteredName.clear();

    QVector<int> roles;
    roles << Qt::UserRole+1;
    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 1), roles);
}

void CheckedSqlListModel::unSelectAll()
{
    setAllChecked(false);
    m_filteredName.clear();

    QVector<int> roles;
    roles << Qt::UserRole+1;
    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 1), roles);
}

QString CheckedSqlListModel::checkedFilterCmd()
{
    QString strRole = roleNames()[Qt::UserRole];

    QStringList filteredString;
    foreach (const QString &name, m_filteredName)
        filteredString << QString("'%1'").arg(name);

    if (m_allChecked  && !filteredString.isEmpty())
    {
        return QString("%1 not in (%2)").arg(strRole).arg(filteredString.join(','));
    }
    else if (!m_allChecked && !filteredString.isEmpty())
    {
        return QString("%1 in (%2)").arg(strRole).arg(filteredString.join(','));
    }

    return QString();
}
