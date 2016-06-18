#include "tablemodel.h"

TableModel::TableModel(QObject *parent):
    AbstractListModel(parent),
    m_roles(),
    m_columnNames(),
    m_rowNames()
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowNames.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnNames.size();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            if (section < m_columnNames.size())
                return m_columnNames.at(section);
            else
                return QVariant::Invalid;
        } else {
            return AbstractListModel::headerData(section, orientation, role);
        }
    } else {
        return AbstractListModel::headerData(section, orientation, role);
    }
}

void TableModel::setRowNames(const QStringList &names)
{
    m_rowNames = names;
}

void TableModel::setColumnNames(const QStringList &names)
{
    m_columnNames.clear();
    m_roles.clear();

    foreach (const QString &name, names)
    {
        m_columnNames << name;
        int index = m_columnNames.size()-1;
        m_roles[Qt::UserRole+index+1] = QVariant::fromValue(headerData(index, Qt::Horizontal)).toByteArray();
    }

    emit rolesChanged();
}

void TableModel::reloadModel()
{
    beginResetModel();
    update_columnNames();
    update_rowNames();
    endResetModel();
}

void TableModel::refreshData()
{
    emit dataChanged(index(0,0), index(rowCount(), columnCount()));
}

QString TableModel::columnColor(const int &index)
{
    if (index > 0)
        return "#804682B4";
    else
        return "transparent";
}

QString TableModel::itemColor(const int &index)
{
    if (index == 0)
        return "#804682B4";
    else
        return "transparent";
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole+1)
    {
        if (index.row() >= 0 && index.row() < m_rowNames.size())
        {
            return m_rowNames.at(index.row());
        }
    }
    else if (m_roles.contains(role) && index.row() < m_rowNames.size())
    {
        return dataItem(m_rowNames.at(index.row()), m_roles[role]);
    }

    return QVariant::Invalid;
}
