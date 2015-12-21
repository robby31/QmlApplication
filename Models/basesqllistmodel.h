#ifndef BASESQLLISTMODEL_H
#define BASESQLLISTMODEL_H

#include "libqmlapplication_global.h"
#include "abstractlistmodel.h"
#include <QtSql>
#include <QDir>

#include <QDebug>

class QMLAPPLICATIONSHARED_EXPORT BaseSqlListModel : public AbstractListModel
{
    Q_OBJECT

    Q_PROPERTY (QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY (QString tablename READ tablename WRITE setTablename NOTIFY tablenameChanged)
    Q_PROPERTY (QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit BaseSqlListModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const { Q_UNUSED(parent) return mRoles.size(); }

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    virtual bool isFiltered() const { return !m_filtercmd.isEmpty(); }
    QString filterCmd() const { return m_filtercmd; }

    QString query();
    QString tablename();
    QString lastError();

    // set the query
    void setQuery(const QString &query);

    // set the tablename
    void setTablename(const QString &name);

    void addCustomRole(const QByteArray &role);

private:
    bool add_role(const QByteArray &roleName);

signals:
    void queryChanged();
    void tablenameChanged();
    void lastErrorChanged();

public slots:
    void reload();

    // update filtering
    void setFilter(const QString &cmd);

private slots:
    // update query based on mStringQuery and fiterCmd
    virtual void update_query();

private:
    QList<QSqlRecord> mRecords;
    QString mStringQuery;
    QSqlQuery mSqlQuery;
    QHash<int, QByteArray> mRoles;
    QString m_tablename;
    QString m_filtercmd;
    QList<QByteArray> m_customRoles;
};

#endif // BASESQLLISTMODEL_H
