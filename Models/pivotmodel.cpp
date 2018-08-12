#include "pivotmodel.h"

PivotModel::PivotModel(QObject *parent):
    SqlListModel(parent),
    m_totalParam()
{
    connect(this, SIGNAL(queryChanged()), this, SIGNAL(totalChanged()));
}

void PivotModel::setTotalParam(const QString &name)
{
    if (!name.isEmpty())
    {
        m_totalParam = name;
        emit totalChanged();
    }
}

double PivotModel::total()
{
    if (m_totalParam.isEmpty())
        return 0.0;

    QString strQuery = query();

    if (!strQuery.isEmpty())
    {
        QString cmd;

        cmd = QString("SELECT sum(%1) FROM (%2)").arg(m_totalParam, strQuery);

        QSqlQuery query(GET_DATABASE(connectionName()));
        if (query.exec(cmd) && query.next())
        {
            return query.record().value(0).toDouble();
        }
    }

    return 0.0;
}
