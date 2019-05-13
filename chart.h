/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CHART_H
#define CHART_H

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
class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    void putData(const QByteArray &data);
    virtual ~Chart();

public slots:
    void handleTimeout();

private:
    QTimer m_timer;
    QLineSeries *ch0;
    QLineSeries *ch1;
    QLineSeries *ch2;
    QLineSeries *ch3;
    QLineSeries *ch4;
    QLineSeries *ch5;
    QLineSeries *ch6;
    QLineSeries *ch7;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_step;
    qreal m_x;
    qreal m_y0;
    qreal m_y1;
    qreal m_y2;
    qreal m_y3;
    qreal m_y4;
    qreal m_y5;
    qreal m_y6;
    qreal m_y7;
//    std::string y0;
//    std::string y1;
//    std::string y2;
//    std::string y3;
//    std::string y4;
//    std::string y5;
//    std::string y6;
//    std::string y7;
    QLineSeries* chList [8];
    qreal y_valList[8];
    void extendAxis(qreal xVal, qreal yVal, QValueAxis* xAxis, QValueAxis* yAxis);
    void convertToDouble(std::string* ADC_strings, qreal* y_valList );
};
//![1]

#endif /* CHART_H */
