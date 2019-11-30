#ifndef MYVXYMODELMAPPER_H
#define MYVXYMODELMAPPER_H

#include <QtCharts>
#include <QAbstractItemModel>
#include "debuginfo.h"

using namespace QtCharts;

class MyVXYModelMapper : public QVXYModelMapper
{
    Q_OBJECT

    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)

public:
    explicit MyVXYModelMapper(QObject *parent = Q_NULLPTR);

    QAbstractAxis *axisX() const;
    bool setAxisX(QAbstractAxis *axis);

    QAbstractAxis *axisY() const;
    bool setAxisY(QAbstractAxis *axis);

private:
    void initAxes();
    QAbstractAxis *getAxisFromType(const QVariant::Type &type);

signals:
    void axisXChanged();
    void axisYChanged();

private slots:
    void _modelReplaced();
    void _modelReset();
    void _insertedRow(const QModelIndex &parent, int first, int last);
    void _xColumnChanged();
    void _yColumnChanged();
    void _analyseData(const int &startRow = -1, const int &endRow = -1);

private:
    QAbstractAxis *m_xAxis = Q_NULLPTR;
    QAbstractAxis *m_yAxis = Q_NULLPTR;
    QAbstractItemModel  *m_model = Q_NULLPTR;
    int m_rowCountAnalysed = -1;
    QVariant m_yMin;
    QVariant m_yMax;
    QVariant m_xMin;
    QVariant m_xMax;
};

#endif // MYVXYMODELMAPPER_H
