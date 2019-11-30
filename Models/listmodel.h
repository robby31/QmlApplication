#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QDebug>
#include "abstractlistmodel.h"
#include "listitem.h"

class ListModel : public AbstractListModel
{
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(ListModel)

public:
    explicit ListModel(ListItem *prototype, QObject *parent = Q_NULLPTR);
    explicit ListModel(QObject *parent = Q_NULLPTR);
    ~ListModel() Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE { Q_UNUSED(parent) return 1; }
    Q_INVOKABLE bool isEmpty() const {if (isFiltered()) return m_filteredIndex.isEmpty(); return m_list.isEmpty();}

    Q_INVOKABLE virtual ListItem *at(int row) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    Q_INVOKABLE ListItem *find(const QString &id) const;
    Q_INVOKABLE int findRow(const QString &id) const;

    QModelIndex indexFromItem(const ListItem *item) const;

    Q_INVOKABLE void clear();

    ListItem *takeRow(int row);
    void appendRow(ListItem *item) { appendRows(QList<ListItem*>() << item); }
    void appendRows(const QList<ListItem*> &items);
    void insertRow(int row, ListItem *item);
    Q_INVOKABLE bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    bool isFiltered() const Q_DECL_OVERRIDE { return m_isFiltered; }
    Q_INVOKABLE void filterRole(const QString &text, const QString &role) { emit filterRoleSignal(text, role); }

private:
    void setFlagFiltered(const bool &flag);

    void insertItem(ListItem *item, int row = -1);

signals:
    void filterRoleSignal(const QString &text, const QString &role);

private slots:
    void itemDestroyed(QObject *item);
    void itemDataChanged(QVector<int> roles);

    void setFilter(const QList<int> &l_index) { beginResetModel(); m_filteredIndex = l_index; setFlagFiltered(true);  endResetModel(); }
    void filterRoleSlot(const QString &text, const QString &role);
    void clearFilter();

private:
    ListItem *m_prototype = Q_NULLPTR;
    bool m_isFiltered = false;
    QList<int> m_filteredIndex;

protected:
    QList<ListItem*> m_list;
};

#endif // LISTMODEL_H
