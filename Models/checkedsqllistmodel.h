#ifndef CHECKEDSQLLISTMODEL_H
#define CHECKEDSQLLISTMODEL_H

#include "Models/sqllistmodel.h"

class CheckedSqlListModel : public SqlListModel
{
    Q_OBJECT

    Q_PROPERTY (QString parameter READ parameter WRITE setParameter NOTIFY parameterChanged)
    Q_PROPERTY (bool allchecked READ allchecked WRITE setAllChecked NOTIFY allcheckedChanged)

public:
    explicit CheckedSqlListModel(QObject *parent = 0);

    QString parameter() const { return m_parameter; }
    void setParameter(const QString &value) { m_parameter = value; emit parameterChanged(); }

    bool allchecked() const { return m_allChecked; }
    void setAllChecked(const bool &flag) { m_allChecked = flag; emit allcheckedChanged(); }

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void unSelectAll();

    Q_INVOKABLE QString checkedFilterCmd();

signals:
    void parameterChanged();
    void allcheckedChanged();

public slots:
    void update_query();

private:
    QString m_parameter;
    bool m_allChecked;
    QStringList m_filteredName;
};

#endif // CHECKEDSQLLISTMODEL_H
