#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QtDebug>
#include <QtSql>
#include "mysqldatabase.h"

class SqlTableModel : public QSqlTableModel
{
    Q_OBJECT

    Q_PROPERTY(QString table READ table WRITE setTableName NOTIFY tableChanged)
    Q_PROPERTY(QString query READ _query WRITE _setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString filter READ _filter WRITE setFilterTable NOTIFY filterChanged)
    Q_PROPERTY(int rowCount READ _rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ _columnCount NOTIFY columnCountChanged)

public:
    explicit SqlTableModel(QObject *parent = Q_NULLPTR);
    explicit SqlTableModel(const QString &connectionName, QObject *parent = Q_NULLPTR);

    QString table() const;
    void setTableName(const QString &name);

    QString _query() const;
    void _setQuery(const QString &cmd);

    QString _filter() const;
    void setFilterTable(const QString &filter);

    int _rowCount() const;
    int _columnCount() const;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

protected:
    void queryChange() Q_DECL_OVERRIDE;
    QString selectStatement() const Q_DECL_OVERRIDE;

private:
    void _initRoles();

signals:
    void tableChanged();
    void queryChanged();
    void filterChanged();
    void rowCountChanged();
    void columnCountChanged();

public slots:
    virtual bool remove(const int &index, const int &count = 1);
    bool submit() Q_DECL_OVERRIDE;

private slots:
    void dataUpdated(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

private:
    QString m_customQuery;
    QHash<int, QByteArray> m_rolesNames;

    bool m_submitFlag = false;
    QList<int> m_submittedRows;
};

#endif // SQLTABLEMODEL_H
