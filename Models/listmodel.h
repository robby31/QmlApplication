#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QDebug>
#include "abstractlistmodel.h"
#include "listitem.h"

class ListModel : public AbstractListModel
{
    Q_OBJECT

public:
    explicit ListModel(ListItem* prototype, QObject* parent = 0);
    explicit ListModel(QObject* parent = 0);
    virtual ~ListModel();

    virtual QHash<int, QByteArray> roleNames() const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const { Q_UNUSED(parent) return 1; }
    Q_INVOKABLE bool isEmpty() const {if (isFiltered()) return m_filteredIndex.isEmpty(); else return m_list.isEmpty();}

    Q_INVOKABLE virtual ListItem *at(int row) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Q_INVOKABLE ListItem* find(const QString &id) const;
    Q_INVOKABLE int findRow(const QString &id) const;

    QModelIndex indexFromItem(const ListItem* item) const;

    void clear();

    ListItem *takeRow(int row);
    void appendRow(ListItem* item) { appendRows(QList<ListItem*>() << item); }
    void appendRows(const QList<ListItem*> &items);
    void insertRow(int row, ListItem* item);
    Q_INVOKABLE bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual bool isFiltered() const { return m_isFiltered; }
    Q_INVOKABLE void filterRole(const QString &text, const QString &role) { emit filterRoleSignal(text, role); }

private:
    void setFlagFiltered(const bool &flag);

    void insertItem(ListItem *item, int row = -1);

signals:
    void filterRoleSignal(const QString &text, const QString &role);

private slots:
    void itemDataChanged(QVector<int> roles);

    void setFilter(QList<int> l_index) { beginResetModel(); m_filteredIndex = l_index; setFlagFiltered(true);  endResetModel(); }
    void filterRoleSlot(const QString &text, const QString &role);
    void clearFilter();

protected:
    ListItem* m_prototype;
    QList<ListItem*> m_list;
    bool m_isFiltered;
    QList<int> m_filteredIndex;
};

#endif // LISTMODEL_H
