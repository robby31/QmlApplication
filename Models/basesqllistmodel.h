#ifndef BASESQLLISTMODEL_H
#define BASESQLLISTMODEL_H

#include "mysqldatabase.h"
#include "abstractlistmodel.h"
#include <QtSql>
#include <QDir>
#include <QElapsedTimer>
#include <QDebug>

class BaseSqlListModel : public AbstractListModel
{
    Q_OBJECT

    Q_PROPERTY (QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY (QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY (QString tablename READ tablename WRITE setTablename NOTIFY tablenameChanged)
    Q_PROPERTY (QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY (QString orderBy READ orderBy WRITE setOrderBy NOTIFY orderByChanged)

public:
    explicit BaseSqlListModel(QObject *parent = 0);

    QVariant getPragma(const QString &param);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    Q_INVOKABLE int append(const QVariantMap &data);
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE bool remove(int row);

    virtual bool isFiltered() const { return !m_filtercmd.isEmpty(); }
    QString filterCmd() const { return m_filtercmd; }

    QString connectionName();
    QString query();
    QString tablename();
    QString lastError();

    // set connection name
    void setConnectionName(const QString &name);

    // set the query
    void setQuery(const QString &query);

    // set the tablename
    void setTablename(const QString &name);

    void addCustomRole(const QByteArray &role);

    QString orderBy() const;
    void setOrderBy(const QString &cmd);

private:
    bool add_role(const QByteArray &roleName);

    bool readForeignKeys();

signals:
    void connectionNameChanged();
    void queryChanged();
    void tablenameChanged();
    void lastErrorChanged();
    void orderByChanged();

public slots:
    void reload();

    // update filtering
    void setFilter(const QString &cmd);

private slots:
    // update query based on mStringQuery and fiterCmd
    virtual void update_query();

private:
    QList<QSqlRecord> mRecords;
    QString mconnectionName;
    QString mStringQuery;
    QSqlQuery mSqlQuery;
    QHash<int, QByteArray> mRoles;
    QString m_tablename;
    QString m_filtercmd;
    QList<QByteArray> m_customRoles;
    QHash<QString, QHash<QString, QHash<QString, QString> > > m_foreignKeys;
    QString m_orderBy;
};

#endif // BASESQLLISTMODEL_H
