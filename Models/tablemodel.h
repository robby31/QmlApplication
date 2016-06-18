#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "libqmlapplication_global.h"
#include "abstractlistmodel.h"

class QMLAPPLICATIONSHARED_EXPORT TableModel : public AbstractListModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = 0);

    virtual QHash<int,QByteArray> roleNames() const { return m_roles; }

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant dataItem(const QString &rowName, const QString &columnName) const = 0;

    Q_INVOKABLE virtual QString columnColor(const int &index);
    Q_INVOKABLE virtual QString itemColor(const int &index);
    Q_INVOKABLE virtual bool isEditable(const int &rowIndex) { Q_UNUSED(rowIndex) return false; }

    virtual bool isFiltered() const { return false; }

    void setRowNames(const QStringList &names);
    void setColumnNames(const QStringList &names);

private:
    virtual void update_rowNames() = 0;
    virtual void update_columnNames() = 0;

signals:
    void rolesChanged();

public slots:
    void reloadModel();
    void refreshData();

private:
    QHash<int, QByteArray> m_roles;
    QStringList m_columnNames;
    QStringList m_rowNames;
};

#endif // TABLEMODEL_H
