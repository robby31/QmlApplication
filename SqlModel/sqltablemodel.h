#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QtDebug>
#include <QtSql>
#include "mysqldatabase.h"

class SqlTableModel : public QSqlTableModel
{
    Q_OBJECT

    Q_PROPERTY(QString table READ tableName WRITE setTableName NOTIFY tableChanged)

    Q_PROPERTY(QString query READ _query WRITE _setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString filter READ _filter WRITE setFilterTable NOTIFY filterChanged)
    Q_PROPERTY(QString orderClause READ orderByClause WRITE setOrderClause NOTIFY orderByClauseChanged)
    Q_PROPERTY(QString group READ group WRITE setGroup NOTIFY groupChanged)
    Q_PROPERTY(QString join READ join WRITE setJoin NOTIFY joinChanged)

    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

    Q_PROPERTY(int rowCount READ _rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ _columnCount NOTIFY columnCountChanged)

public:
    explicit SqlTableModel(QObject *parent = Q_NULLPTR);
    explicit SqlTableModel(const QString &connectionName, QObject *parent = Q_NULLPTR);

    void setTableName(const QString &name);

    QString _query() const;
    void _setQuery(const QString &cmd);

    QString _filter() const;
    void setFilterTable(const QString &filter);

    void setOrderClause(const QString &stmt);

    QString group() const;
    void setGroup(const QString &group);

    QString join() const;
    void setJoin(const QString &join);

    QString error() const;

    int _rowCount() const;
    int _columnCount() const;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

protected:
    void queryChange() Q_DECL_OVERRIDE;
    QString selectStatement() const Q_DECL_OVERRIDE;

    QString orderByClause() const Q_DECL_OVERRIDE;

private:
    void _initRoles();

signals:
    void tableChanged();
    void queryChanged();
    void filterChanged();
    void orderByClauseChanged();
    void groupChanged();
    void joinChanged();
    void errorChanged();
    void rowCountChanged();
    void columnCountChanged();

public slots:
    virtual bool remove(const int &index, const int &count = 1);

private:
    QString m_customQuery;
    QString m_customOrder;
    QString m_customGroup;
    QString m_customJoin;
    QHash<int, QByteArray> m_rolesNames;
};

#endif // SQLTABLEMODEL_H
