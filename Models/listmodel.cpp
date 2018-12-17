#include "listmodel.h"
#include <QDebug>

ListModel::ListModel(ListItem* prototype, QObject *parent) :
    AbstractListModel(parent),
    m_prototype(prototype),
    m_isFiltered(false)
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    connect(this, SIGNAL(filterRoleSignal(QString,QString)), this, SLOT(filterRoleSlot(QString,QString)));
}

ListModel::ListModel(QObject *parent) :
    AbstractListModel(parent),
    m_prototype(),
    m_isFiltered(false)
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    connect(this, SIGNAL(filterRoleSignal(QString,QString)), this, SLOT(filterRoleSlot(QString,QString)));
}

ListModel::~ListModel()
{
    m_filteredIndex.clear();
    qDeleteAll(m_list);
    delete m_prototype;
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    if (m_prototype)
        return m_prototype->roleNames();

    return QHash<int, QByteArray>();
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (!isFiltered())
        return m_list.size();

    return m_filteredIndex.size();
}

ListItem *ListModel::at(int row) const
{
    if (!isFiltered())
        return m_list.at(row);

    return m_list.at(m_filteredIndex.at(row));
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (index.row()>=0 && index.row()<rowCount())
    {
        if (role == Qt::DisplayRole)
        {
            return data(index, Qt::UserRole+index.column()+1);
        }

        if (roleNames().contains(role))
        {
            ListItem *item = at(index.row());
            if (item)
                return item->data(role);
        }

        qCritical() << "invalid role" << role << roleNames().keys();
    }

    return QVariant::Invalid;
}

ListItem * ListModel::find(const QString &id) const
{
    for (int index=0; index<rowCount(); ++index)
    {
        ListItem *item = at(index);
        if (item && item->id() == id)
            return item;
    }
    return Q_NULLPTR;
}

int ListModel::findRow(const QString &id) const
{
    ListItem *item = find(id);
    return indexFromItem(item).row();
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
        return false;

    QHash<int, QByteArray> roles = roleNames();
    if (roles.contains(role))
    {
        ListItem *item = at(index.row());
        if (item)
            return item->setData(value, role);
    }
    return false;
}

QModelIndex ListModel::indexFromItem(const ListItem *item) const
{
    if (item)
    {
        for (int row=0; row<rowCount(); ++row) {
            if (at(row) == item)
                return indexFromRow(row);
        }
    }

    return {};
}

void ListModel::insertItem(ListItem *item, int row)
{
    if (item)
    {
        if (item->parent() != this)
        {
            if (item->parent())   // parent not null
                qWarning() << "items in a ListModel shall have model as parent" << item->parent() << "!=" << this;
            item->setParent(this);    // items shall have model as parent
        }

        if (item->parent() == this)
        {
            if (row>=0)
                m_list.insert(row, item);
            else
                m_list.append(item);
            connect(item, SIGNAL(itemChanged(QVector<int>)), this, SLOT(itemDataChanged(QVector<int>)));
        }
        else
        {
            if (row>=0)
                m_list.insert(row, Q_NULLPTR);
            else
                m_list.append(Q_NULLPTR);
            qWarning() << "cannot append" << item << "in the model" << this << "(null item added)";
        }
    }
    else
    {
        qWarning() << "cannot add null item";
        if (row>=0)
            m_list.insert(row, Q_NULLPTR);
        else
            m_list.append(Q_NULLPTR);
    }
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(ListItem *item, items)
        insertItem(item);
    endInsertRows();
}

void ListModel::insertRow(int row, ListItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    insertItem(item, row);
    endInsertRows();
}

void ListModel::clear()
{
    if (!m_list.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, m_list.size()-1);
        qDeleteAll(m_list);
        m_list.clear();
        clearFilter();
        endRemoveRows();
    }
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (isFiltered())
    {
        qWarning() << "unable to remove row in filtered model";
        return false;
    }

    if(row < 0 || row >= rowCount()) return false;
    beginRemoveRows(QModelIndex(), row, row);

    ListItem *item = m_list.takeAt(row);
    if (item)
        item->deleteLater();

    endRemoveRows();
    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (isFiltered())
    {
        qWarning() << "unable to remove row in filtered model";
        return false;
    }

    if(row < 0 || (row+count) > rowCount()) return false;
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int i=0; i<count; ++i) {
        ListItem *item = m_list.takeAt(row);
        if (item)
            item->deleteLater();
    }
    endRemoveRows();
    return true;
}

ListItem *ListModel::takeRow(int row)
{
    if (isFiltered())
    {
        qWarning() << "unable to remove row in filtered model";
        return Q_NULLPTR;
    }

    beginRemoveRows(QModelIndex(), row, row);
    ListItem* item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}

void ListModel::itemDataChanged(QVector<int> roles)
{
    auto item = qobject_cast<ListItem*>(sender());
    if (item)
    {
        auto min_max = std::minmax_element(roles.begin(), roles.end());
        int minRole = *min_max.first;
        int maxRole = *min_max.second;

        QModelIndex topLeftIndex = indexFromItem(item);
        if (topLeftIndex.isValid())
        {
            QModelIndex bottomRightIndex = topLeftIndex;

            if (minRole > Qt::UserRole)
                topLeftIndex = index(topLeftIndex.row(), minRole-Qt::UserRole-1);
            if (maxRole > Qt::UserRole)
                bottomRightIndex = index(bottomRightIndex.row(), maxRole-Qt::UserRole-1);

            if (!roles.isEmpty())
                roles << Qt::DisplayRole;

            if (topLeftIndex.isValid() && bottomRightIndex.isValid())
                emit dataChanged(topLeftIndex, bottomRightIndex, roles);
            else
                qCritical() << "invalid index" << topLeftIndex << bottomRightIndex << "in item data changed" << roles;
        }
    }
}

void ListModel::clearFilter()
{
    if (isFiltered())
    {
        beginResetModel();
        m_filteredIndex.clear();
        setFlagFiltered(false);
        endResetModel();
    }
}

void ListModel::setFlagFiltered(const bool &flag)
{
    if (m_isFiltered != flag)
    {
        m_isFiltered = flag;
        emit isFilteredChanged();
    }
}

void ListModel::filterRoleSlot(const QString &text, const QString &role)
{
    int roleIndex = roleNames().key(role.toLatin1());

    if (roleNames().contains(roleIndex))
    {
        QList<int> newIndex;
        for (int row=0; row<m_list.size(); ++row)
        {
            ListItem *item = m_list.at(row);
            if (item && item->data(roleIndex).toString().toLower().contains(text.toLower()))
                newIndex.append(row);
        }
        setFilter(newIndex);
    }
    else
    {
        qWarning() << "ERROR invalid role" << role << "to filter" << this;
    }
}
