#include "imetPlot.h"
#include "ui_mainwindow.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

imetPlot::imetPlot(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),

    pres(nullptr),
    temp(nullptr),
    humd(nullptr),
    rtemp(nullptr),

    m_axisX(new QValueAxis()),
    m_axisYpres(new QValueAxis()),
    m_axisYtemp(new QValueAxis()),
    m_axisYhumd(new QValueAxis()),
    m_step(0),
    m_x(0),

    m_ypres(0),
    m_ytemp(0),
    m_yhumd(0),
    m_yrtemp(0)
{
   // Set chart to update every second
    QObject::connect(&m_timer, &QTimer::timeout, this, &imetPlot::handleTimeout);
    m_timer.setInterval(1000);

    //Initialize members
    pres = new QLineSeries(this);
    iMetFieldList[0] = pres;
    temp = new QLineSeries(this);
    iMetFieldList[1] = temp;
    humd = new QLineSeries(this);
    iMetFieldList[2] = humd;
    rtemp = new QLineSeries(this);
    iMetFieldList[3] = rtemp;

    iMet_y_valList[0] = m_ypres;
    iMet_y_valList[1] = m_ytemp;
    iMet_y_valList[2] = m_yhumd;
    iMet_y_valList[3] = m_yrtemp;

    QPen red(Qt::red);
    QPen green(Qt::green);
    QPen blue(Qt::blue);
    QPen yellow(Qt::yellow);



    QPen penList [] = {red, green, blue, yellow};
    m_titles << "Pressure (hPa)" << "Temperature (C)" << "Humidity (%)" << "Relative Temperature (C)";

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisYpres,Qt::AlignLeft);
    addAxis(m_axisYhumd,Qt::AlignLeft);
    addAxis(m_axisYtemp,Qt::AlignRight);

    for(int i = 0; i < 4; i++ ){

        iMetFieldList[i]->setName(m_titles[i]);

        //Initialize the first data point
        iMetFieldList[i]->append(m_x, iMet_y_valList[i]);

        // Set the line properties
        penList[i].setWidth(3);
        iMetFieldList[i]->setPen(penList[i]);

        // Add data series to the chart, attach to axes
        addSeries(iMetFieldList[i]);
        iMetFieldList[i]->attachAxis(m_axisX);
    }

    iMetFieldList[0]->attachAxis(m_axisYpres);
    iMetFieldList[1]->attachAxis(m_axisYtemp);
    iMetFieldList[2]->attachAxis(m_axisYhumd);
    iMetFieldList[3]->attachAxis(m_axisYtemp);

    m_axisX->setTickCount(5);
    m_axisX->setRange(0, 60);
    m_axisYpres->setRange(0, 1000);
    m_axisYtemp->setRange(0, 40);
    m_axisYhumd->setRange(0, 1);

    m_timer.start();
}

imetPlot::~imetPlot()
{

}

void imetPlot::handleTimeout()
{
    m_x++;

    // Update each channel every second
    for(int i = 0; i< 4; i++){
        iMetFieldList[i]->append(m_x,iMet_y_valList[i]);
        switch (i) {
            case 0:
                extendAxis(m_x, iMet_y_valList[i], m_axisX, m_axisYpres);
                break;
            case 2:
                extendAxis(m_x, iMet_y_valList[i], m_axisX, m_axisYhumd);
                break;
            default:
                extendAxis(m_x, iMet_y_valList[i], m_axisX, m_axisYtemp);
                break;

        }

    }
}

void imetPlot::putData(const QByteArray &data){

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
                //ADC[chStr].assign(dataStr, begin, i-begin);
                //qInfo() << ADC[chStr].c_str();
                begin = i+1;
                chStr++;
                continue;
        }
    }


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
                   begin = i+1;
                   iMetField++;
                   continue;
           }
       }
   }

   convertiMet(iMet, iMet_y_valList);
   iMet_y_valList[0] = iMet_y_valList[0]/100;
   iMet_y_valList[1] = iMet_y_valList[1]/100;
   iMet_y_valList[2] = iMet_y_valList[2]/1000;
   iMet_y_valList[3] = iMet_y_valList[3]/100;
   return;
}

void imetPlot::extendAxis(qreal xVal, qreal yVal, QValueAxis* xAxis, QValueAxis* yAxis){
    if(yVal < yAxis->min()){
       yAxis->setMin(yVal);
    }
    else if ( yVal > yAxis->max()) {
        yAxis->setMax(yVal);
    }
    if(xVal > xAxis->max() -5){
        xAxis->setMax(xAxis->max()+1);
    }
}

void imetPlot::convertiMet(std::string* iMet_strings, qreal* y_valList ){
    for(int i = 0; i < 4; i++){
        try {
            y_valList[i] = std::stod(iMet_strings[i+1],nullptr);
        } catch (...) {
            qInfo() << "Unable to convert iMet value to floating point. Discarding message.";
            return;
        }
    }
}
