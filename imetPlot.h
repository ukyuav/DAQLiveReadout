#ifndef IMETPLOT_H
#define IMETPLOT_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <string>
#include <iostream>
#include <sstream>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

//![1]
class imetPlot: public QChart
{
    Q_OBJECT
public:
    imetPlot(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    void putData(const QByteArray &data);
    virtual ~imetPlot();

public slots:
    void handleTimeout();

private:
    QTimer m_timer;
    QLineSeries *pres;
    QLineSeries *temp;
    QLineSeries *humd;
    QLineSeries *rtemp;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisYpres;
    QValueAxis *m_axisYtemp;
    QValueAxis *m_axisYhumd;
    qreal m_step;
    qreal m_x;
    qreal m_ypres;
    qreal m_ytemp;
    qreal m_yhumd;
    qreal m_yrtemp;
    QLineSeries* iMetFieldList[8];
    qreal iMet_y_valList[8];
    void extendAxis(qreal xVal, qreal yVal, QValueAxis* xAxis, QValueAxis* yAxis);
    void convertiMet(std::string* iMet_strings, qreal* y_valList );
};
//![1]
//!
#endif // IMETPLOT_H
