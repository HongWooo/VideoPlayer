#include "chart.h"
#include "ui_chart.h"
#include "QtCharts/QChart"
#include "QLineSeries"
#include "QValueAxis"
#include "QTimer"
#include "QTime"
#include "QList"
#include "qmath.h"
#include "QPointF"
#include "QDebug"
#include "QChartView"
#include <QRandomGenerator>

//#include <QtCharts/QChartGlobal>

QT_CHARTS_USE_NAMESPACE


QChart *m_chart;
QLineSeries *m_series;
//QList<double> dataList;//存储业务数据
int maxSize = 1000;

//QTimer updateTimer;
int timeId;


Chart::Chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chart)
{
    ui->setupUi(this);

    m_chart = new QChart;
    QChartView *chartView = new QChartView(m_chart);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
//    chartView->setRubberBand();

    m_series = new QLineSeries;
    m_series->setPen(QPen(Qt::red,2,Qt::SolidLine));
    m_chart->addSeries(m_series);

    for(int i=0;i<maxSize;++i){
       m_series->append(i,0);
    }
    m_series->setUseOpenGL(true);//openGl 加速
    qDebug()<<m_series->useOpenGL();

    QValueAxis *axisX = new QValueAxis;

    axisX->setRange(0,maxSize);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Time");

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,1.0);
    axisY->setTitleText("异常概率");

    m_chart->setAxisX(axisX,m_series);
    m_chart->setAxisY(axisY,m_series);
    m_chart->legend()->hide();
    m_chart->setTitle("异常风险实际检测");

    QVBoxLayout *layout = ui->chart_holder;
    layout->addWidget(chartView);

//    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot()));
//    updateTimer.start(0);
    timeId = startTimer(150);
}

Chart::~Chart()
{
    delete ui;
}

double Chart::getData(double time){

//    double s = qCos( time * M_PI * 2 ) ;
    double x = QRandomGenerator::global()->bounded(0.1)-0.1;
    double rand = QRandomGenerator::global()->bounded(0.1+x);
//    double s = 1/(1+qExp(-time+10)) ;
       qDebug()<<time<<endl;
//    double a = -0.560;
//    double b = -0.238;
//    double c = 0.836;
//    double d = 0.562;
//    double x = qCos( time/15 * M_PI * 2 );
//    qDebug()<<"x="<<time<<endl;
//    qDebug()<<"rand = "<<rand<<endl;
//    double s = a*qPow(x,3)+b*qPow(x,2)+c*x+d + rand;

    if(time>8 && time <8.2)
        rand = rand +pow(4*(time-8),4);
    if(time >=8.2 && time <8.4)
        rand = rand +0.8+x;
    return rand;
}

void Chart::timerEvent(QTimerEvent *event){
    if(event->timerId()==timeId){//定时器到时间,//模拟数据填充
        static QTime dataTime(QTime::currentTime());
        long int eltime = dataTime.elapsed();
        static int lastpointtime = 0;
        int size = (eltime - lastpointtime);//数据个数
//        qDebug()<<"last time-->"<<QTime::currentTime()<<endl;
//        qDebug()<<"size-->"<<size;
        if(isVisible()){
            QVector<QPointF> oldPoints = m_series->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points;

            for(int i=size;i<oldPoints.count();++i){
                points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
            }
            qint64 sizePoints = points.count();
            for(int k=0;k<size;++k){
                points.append(QPointF(k+sizePoints,getData((((double)lastpointtime+k+1)/1000))));
            }
            m_series->replace(points);
            lastpointtime = eltime;
       }
    }
}
