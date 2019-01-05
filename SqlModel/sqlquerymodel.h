#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QtDebug>
#include <QtSql>
#include "mysqldatabase.h"

#include <QDateTime>

class SqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(QString query READ _query WRITE _setQuery NOTIFY queryChanged)
    Q_PROPERTY(int rowCount READ _rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ _columnCount NOTIFY columnCountChanged)

public:
    explicit SqlQueryModel(QObject *parent = Q_NULLPTR);

    QString connectionName() const;
    void setConnectionName(const QString &name);

    QString _query() const;
    void _setQuery(const QString &cmd);

    int _rowCount() const;
    int _columnCount() const;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

protected:
    void queryChange() Q_DECL_OVERRIDE;

private:
    void _initRoles();

signals:
    void connectionNameChanged();
    void queryChanged();
    void rowCountChanged();
    void columnCountChanged();

public slots:
    void reload();

private slots:
    void _updateCurrentQuery();

private:
    QString m_connectionName;
    QString m_tmpQuery;
    QHash<int, QByteArray> m_rolesNames;
};

#endif // SQLQUERYMODEL_H
