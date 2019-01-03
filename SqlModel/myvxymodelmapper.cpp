#include "myvxymodelmapper.h"

MyVXYModelMapper::MyVXYModelMapper(QObject *parent):
    QVXYModelMapper(parent)
{
    connect(this, &MyVXYModelMapper::modelReplaced, this, &MyVXYModelMapper::_modelReplaced);
    connect(this, &MyVXYModelMapper::xColumnChanged, this, &MyVXYModelMapper::_xColumnChanged);
    connect(this, &MyVXYModelMapper::yColumnChanged, this, &MyVXYModelMapper::_yColumnChanged);
    connect(this, &MyVXYModelMapper::rowCountChanged, this, &MyVXYModelMapper::_analyseData);
}

void MyVXYModelMapper::_modelReplaced()
{
    qDebug() << "model replaced" << m_model << model();

    if (m_model)
    {
        m_model->disconnect(this);
        disconnect(m_model);
    }

    m_rowCountAnalysed = -1;
    m_xMin.clear();
    m_xMax.clear();
    m_yMin.clear();
    m_yMax.clear();

    m_model = model();
    if (m_model)
    {
        connect(m_model, &QAbstractItemModel::modelReset, this, &MyVXYModelMapper::_modelReset);
    }
}

void MyVXYModelMapper::_modelReset()
{
    qDebug() << "model reset" << m_model;

    m_rowCountAnalysed = -1;
    m_xMin.clear();
    m_xMax.clear();
    m_yMin.clear();
    m_yMax.clear();
    initAxes();
    _analyseData();
}

void MyVXYModelMapper::_xColumnChanged()
{
    qDebug() << "x column changed" << xColumn();

    m_rowCountAnalysed = -1;
    m_xMin.clear();
    m_xMax.clear();
    initAxes();
    _analyseData();
}

void MyVXYModelMapper::_yColumnChanged()
{
    qDebug() << "y column changed" << yColumn();

    m_rowCountAnalysed = -1;
    m_yMin.clear();
    m_yMax.clear();
    initAxes();
    _analyseData();
}

void MyVXYModelMapper::_analyseData()
{
    if (m_rowCountAnalysed != -1 && rowCount() <= m_rowCountAnalysed)
    {
        qWarning() << "data already analysed" << m_rowCountAnalysed << rowCount();
        return;
    }

    if (!m_model || xColumn()<0 || yColumn()<0)
    {
        qDebug() << "unable analyse data" << m_model << xColumn() << yColumn();
        return;
    }

    qDebug() << m_rowCountAnalysed << "analyse data, model size" << rowCount();

    int init_row = 0;
    if (m_rowCountAnalysed > 0)
        init_row = m_rowCountAnalysed;

    for (int row = init_row;row<rowCount();++row)
    {
        QVariant xValue = m_model->data(m_model->index(row, xColumn()));
        QVariant yValue = m_model->data(m_model->index(row, yColumn()));

        if (!m_xMin.isValid() || xValue < m_xMin)
            m_xMin = xValue;

        if (!m_xMax.isValid() || xValue > m_xMax)
            m_xMax = xValue;

        if (!m_yMin.isValid() || yValue < m_yMin)
           m_yMin = yValue;

        if (!m_yMax.isValid() || yValue > m_yMax)
            m_yMax = yValue;
    }

    m_rowCountAnalysed = rowCount();

    qDebug() << "define x min max from data" << m_xMin << m_xMax;
    if (m_xAxis)
    {
        if (m_xAxis->type() == QtCharts::QAbstractAxis::AxisTypeValue)
        {
            if (m_xMin == m_xMax)
            {
                m_xMin = QVariant::fromValue(m_xMin.toInt() - 1);
                m_xMax = QVariant::fromValue(m_xMax.toInt() + 1);
            }
        }

        m_xAxis->setRange(m_xMin, m_xMax);

        if (m_xMin.type() == QVariant::Int or m_xMin.type() == QVariant::LongLong)
        {
            auto tmpAxis = qobject_cast<QValueAxis*>(m_xAxis);
            tmpAxis->setTickCount(m_xMax.toInt()-m_xMin.toInt()+1);
        }
    }

    qDebug() << "define y min max from data" << m_yMin << m_yMax;
    if (m_yAxis)
    {
        if (m_yAxis->type() == QtCharts::QAbstractAxis::AxisTypeValue)
        {
            if (m_yMin>0)
                m_yMin = 0;
            if (m_yMax<0)
                m_yMax = 0;
        }

        m_yAxis->setRange(m_yMin, m_yMax);

        auto tmpAxis = qobject_cast<QValueAxis*>(m_yAxis);
        if (tmpAxis)
        {
            qDebug() << "apply nice numbers";
            tmpAxis->applyNiceNumbers();
        }
    }
}

QAbstractAxis *MyVXYModelMapper::axisX() const
{
    return m_xAxis;
}

QAbstractAxis *MyVXYModelMapper::axisY() const
{
    return m_yAxis;
}

void MyVXYModelMapper::initAxes()
{
    if (m_model)
    {
        if (xColumn()>=0)
        {
            QVariant xValue = m_model->data(m_model->index(0, xColumn()));
            qDebug() << "init AXES" << xValue << "type" << xValue.type();
            auto axis = getAxisFromType(xValue.type());
            if (!setAxisX(axis))
                delete axis;

            if (m_xAxis)
            {
                m_xAxis->setTitleText(m_model->headerData(xColumn(), Qt::Horizontal).toString());

                QFont font(m_xAxis->titleFont());
                font.setCapitalization(QFont::Capitalize);
                m_xAxis->setTitleFont(font);

                if (m_xAxis->type() == QtCharts::QAbstractAxis::AxisTypeValue)
                {
                    auto tmpAxis = qobject_cast<QValueAxis*>(m_xAxis);
                    if (xValue.type() == QVariant::Int or xValue.type() == QVariant::LongLong)
                    {
                        tmpAxis->setLabelFormat("%d");
                    }
                }
            }
        }

        if (yColumn()>=0)
        {
            QVariant yValue = m_model->data(m_model->index(0, yColumn()));
            qDebug() << "init AXES" << yValue << "type" << yValue.type();
            auto axis = getAxisFromType(yValue.type());
            if (!setAxisY(axis))
                delete axis;

            if (m_yAxis)
            {
                m_yAxis->setTitleText(m_model->headerData(yColumn(), Qt::Horizontal).toString());

                QFont font(m_yAxis->titleFont());
                font.setCapitalization(QFont::Capitalize);
                m_yAxis->setTitleFont(font);
            }
        }
    }
}

QAbstractAxis *MyVXYModelMapper::getAxisFromType(const QVariant::Type &type)
{
    QAbstractAxis *axis = Q_NULLPTR;

    if (type == QVariant::DateTime)
    {
        auto tmp = new QDateTimeAxis(this);
        tmp->setFormat("MM yyyy");
        axis = tmp;
    }
    else
    {
        axis = new QValueAxis(this);
    }

    return axis;
}

bool MyVXYModelMapper::setAxisX(QAbstractAxis *axis)
{
    qDebug() << "set Axis X" << axis;

    if (!m_xAxis or m_xAxis->type() != axis->type())
    {
        if (m_xAxis)
            m_xAxis->deleteLater();
        m_xAxis = axis;
        emit axisXChanged();
        return true;
    }

    return false;
}

bool MyVXYModelMapper::setAxisY(QAbstractAxis *axis)
{
    qDebug() << "set Axis Y" << axis;

    if (!m_yAxis or m_yAxis->type() != axis->type())
    {
        if (m_yAxis)
            m_yAxis->deleteLater();
        m_yAxis = axis;
        emit axisYChanged();
        return true;
    }

    return false;
}
