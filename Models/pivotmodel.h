#ifndef PIVOTMODEL_H
#define PIVOTMODEL_H

#include "Models/sqllistmodel.h"

class PivotModel : public SqlListModel
{
    Q_OBJECT

    Q_PROPERTY(double total READ total NOTIFY totalChanged)

public:
    explicit PivotModel(QObject *parent = Q_NULLPTR);

    double total();
    Q_INVOKABLE void setTotalParam(const QString &name);

signals:
    void totalChanged();

public slots:

private:
    QString m_totalParam;
};

#endif // PIVOTMODEL_H
