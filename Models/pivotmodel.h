#ifndef PIVOTMODEL_H
#define PIVOTMODEL_H

#include "libqmlapplication_global.h"
#include "Models/sqllistmodel.h"

class QMLAPPLICATIONSHARED_EXPORT PivotModel : public SqlListModel
{
    Q_OBJECT

    Q_PROPERTY(double total READ total NOTIFY totalChanged)

public:
    explicit PivotModel(QObject *parent = 0);

    double total();
    Q_INVOKABLE void setTotalParam(const QString &name);

signals:
    void totalChanged();

public slots:

private:
    QString m_totalParam;
};

#endif // PIVOTMODEL_H
