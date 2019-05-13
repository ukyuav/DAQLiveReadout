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

#include "chart.h"
#include "ui_mainwindow.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),

    ch0(nullptr),
    ch1(nullptr),
    ch2(nullptr),
    ch3(nullptr),
    ch4(nullptr),
    ch5(nullptr),
    ch6(nullptr),
    ch7(nullptr),

    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_step(0),
    m_x(0),

    m_y0(0),
    m_y1(0),
    m_y2(0),
    m_y3(0),
    m_y4(0),
    m_y5(0),
    m_y6(0),
    m_y7(0)
{
   // Set chart to update every second
    QObject::connect(&m_timer, &QTimer::timeout, this, &Chart::handleTimeout);
    m_timer.setInterval(1000);

    //Initialize members
    ch0 = new QLineSeries(this);
    chList[0] = ch0;
    ch1 = new QLineSeries(this);
    chList[1] = ch1;
    ch2 = new QLineSeries(this);
    chList[2] = ch2;
    ch3 = new QLineSeries(this);
    chList[3] = ch3;
    ch4 = new QLineSeries(this);
    chList[4] = ch4;
    ch5 = new QLineSeries(this);
    chList[5] = ch5;
    ch6 = new QLineSeries(this);
    chList[6] = ch6;
    ch7 = new QLineSeries(this);
    chList[7] = ch7;

    y_valList[0] = m_y0;
    y_valList[1] = m_y1;
    y_valList[2] = m_y2;
    y_valList[3] = m_y3;
    y_valList[4] = m_y4;
    y_valList[5] = m_y5;
    y_valList[6] = m_y6;
    y_valList[7] = m_y7;

    QPen red(Qt::red);
    QPen green(Qt::green);
    QPen blue(Qt::blue);
    QPen yellow(Qt::yellow);
    QPen gray(Qt::gray);
    QPen black(Qt::black);
    QPen cyan(Qt::cyan);
    QPen magenta(Qt::magenta);


    QPen penList [] = {red, green, blue, yellow, gray, black, cyan, magenta};

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);

    for(int i = 0; i < 8; i++ ){

        // Set name for each channel
        std::ostringstream out;
        out << "Ch " << i;
        chList[i]->setName(out.str().c_str());

        //Initialize the first data point
        chList[i]->append(m_x, y_valList[i]);

        // Set the line properties
        penList[i].setWidth(3);
        chList[i]->setPen(penList[i]);

        // Add data series to the chart, attach to axes
        addSeries(chList[i]);
        chList[i]->attachAxis(m_axisX);
        chList[i]->attachAxis(m_axisY);
    }

    m_axisX->setTickCount(5);
    m_axisX->setRange(0, 60);
    m_axisY->setRange(-1, 1);

    m_timer.start();
}

Chart::~Chart()
{

}

void Chart::handleTimeout()
{
    m_x++;

    // Update each channel every second
    for(int i = 0; i< 8; i++){
        chList[i]->append(m_x,y_valList[i]);
        extendAxis(m_x, y_valList[i], m_axisX, m_axisY);
    }
}

void Chart::putData(const QByteArray &data){

    // Take newest line of data from serial port as string
    std::string dataStr = (std::string) data;
    qInfo() << data;
    std::string ADC[8];

    // Parse string into fields
    int begin = 0;
    int end = 1;
    int chStr = 0;

    // Extract first 8 floats
    for(int i = end; chStr < 8; i++){
        if(data.size()-1 < i){
            qInfo() << "Incomplete packet, discarding.";
            return;
        }
        if(data[i] == ','){
                end = i-1;
                ADC[chStr].assign(dataStr, begin, i-begin);
                //qInfo() << ADC[chStr].c_str();
                begin = i+1;
                chStr++;
                continue;
        }
    }

   convertToDouble(ADC ,y_valList);

   // Extract IMET data
   std::string iMet[10];
   int iMetField = 0;
   if( begin +1 > data.size()-1){
       return;
   }
   if((chStr == 8) && (data[begin] == 'X') && (data[begin+1] == 'Q')){
       for(int i = begin; iMetField < 10; i++){
           if(data.size()-1 < i){
               qInfo() << "Incomplete packet, discarding.";
               return;
           }
           if(data[i] == ','){
                   end = i-1;
                   iMet[iMetField].assign(dataStr, begin, i-begin);
                   //qInfo() << iMet[iMetField].c_str();
                   begin = i+1;
                   iMetField++;
                   continue;
           }
       }
   }

  // convertiMet(iMet);
   return;
}

// Rescale the plot as needed
void Chart::extendAxis(qreal xVal, qreal yVal, QValueAxis* xAxis, QValueAxis* yAxis){
    if(yVal < yAxis->min() && yVal > -10){
       yAxis->setMin(yVal);
    }
    else if ( yVal > yAxis->max() && yVal < 10) {
        yAxis->setMax(yVal);
    }
    if(xVal > xAxis->max() -5){
        xAxis->setMax(xAxis->max()+1);
    }
}

// Convert strings into float for each channel
void Chart::convertToDouble(std::string* ADC_strings, qreal* y_valList ){
    for(int i = 0; i < 8; i++){
        try {
            y_valList[i] = std::stod(ADC_strings[i],nullptr);
        } catch (...) {
            qInfo() << "Unable to convert transmitted value to floating point. Discarding message.";
            return;
        }
    }
}

/* void
*/
