#ifndef CHECKEDSQLLISTMODEL_H
#define CHECKEDSQLLISTMODEL_H

#include "basesqllistmodel.h"

class CheckedSqlListModel : public BaseSqlListModel
{
    Q_OBJECT

    Q_PROPERTY (QString queryData READ queryData WRITE setQueryData NOTIFY queryDataChanged)
    Q_PROPERTY (QString parameter READ parameter WRITE setParameter NOTIFY parameterChanged)
    Q_PROPERTY (bool allchecked READ allchecked WRITE setAllChecked NOTIFY allcheckedChanged)
    Q_PROPERTY (QString textFilter READ textFilter WRITE setTextFilter NOTIFY textFilterChanged)

public:
    explicit CheckedSqlListModel(QObject *parent = 0);

    QString parameter() const { return m_parameter; }
    void setParameter(const QString &value);

    bool allchecked() const { return m_allChecked; }
    void setAllChecked(const bool &flag) { m_allChecked = flag; emit allcheckedChanged(); }

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    QString queryData() const { return m_queryData; }
    void setQueryData(const QString &query);

    QString textFilter() const { return m_textFilter; }
    void setTextFilter(const QString &text);

    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void unSelectAll();

    Q_INVOKABLE QString checkedFilterCmd();

signals:
    void queryDataChanged();
    void parameterChanged();
    void allcheckedChanged();
    void textFilterChanged();

private slots:
    void init_query();

private:
    QString m_parameter;
    bool m_allChecked;
    QStringList m_filteredName;
    QString m_textFilter;
    QString m_queryData;
};

#endif // CHECKEDSQLLISTMODEL_H
