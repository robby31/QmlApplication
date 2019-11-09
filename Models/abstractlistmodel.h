#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H

#include <QAbstractItemModel>
#include <QDebug>

class AbstractListModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ columnCount NOTIFY columnCountChanged)
    Q_PROPERTY(bool isFiltered READ isFiltered NOTIFY isFilteredChanged)

public:
    explicit AbstractListModel(QObject *parent = Q_NULLPTR);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE { Q_UNUSED(parent) return createIndex(row, column, quintptr(0)); }
    QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE { Q_UNUSED(child) return {}; }

    Q_INVOKABLE QVariant dataByStringRole(const QModelIndex &index, const QString &role) const;

    Q_INVOKABLE QVariantMap get(const int &index);
    Q_INVOKABLE QVariant get(int i, const QString &role) const;

    Q_INVOKABLE virtual QString role(int columnNumber) const;
    Q_INVOKABLE QString title(int columnNumber) const { return headerData(columnNumber, Qt::Horizontal).toString(); }

    Q_INVOKABLE int findRow(const QVariant &value, const QString &role) const;

    QModelIndex indexFromRow(const int& row) const;

    Q_INVOKABLE bool setDataByStringRole(const int& row, const QVariant &value, const QString &role);
    Q_INVOKABLE bool setDataByStringRole(const QModelIndex &index, const QVariant &value, const QString &role);

    virtual bool isFiltered() const = 0;

signals:
    void rowCountChanged();
    void columnCountChanged();
    void isFilteredChanged();

private slots:

};

#endif // ABSTRACTLISTMODEL_H
