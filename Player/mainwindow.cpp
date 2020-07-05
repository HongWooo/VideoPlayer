#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //无边框
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //背景透明(自己画)
//    setAttribute(Qt::WA_TranslucentBackground);
    //加载自己的logo
    m_logo.load(":/images/logo.png");

    monitor_page = new monitor(this);
    video_page = new Video(this);
    setting_page = new Setting(this);
    chart_page = new Chart(this);
    ui->stackedWidget->addWidget(monitor_page);
    ui->stackedWidget->addWidget(video_page);
    ui->stackedWidget->addWidget(setting_page);
    ui->stackedWidget->addWidget(chart_page);

    ui->stackedWidget->setCurrentWidget(monitor_page);
    ui->btn_monitor->setProperty("pressed",true);

    connect(ui->btn_max,SIGNAL(clicked(bool)),this,SLOT(onMaxClicked(bool)));
    connect(ui->btn_min,SIGNAL(clicked(bool)),this,SLOT(onMinClicked(bool)));
    connect(ui->btn_close,SIGNAL(clicked(bool)),this,SLOT(onCloseClicked(bool)));
    connect(ui->btn_monitor,SIGNAL(clicked(bool)),this,SLOT(onMoniorClicked(bool)));
    connect(ui->btn_video,SIGNAL(clicked(bool)),this,SLOT(onVideoBtnClicked(bool)));
    connect(ui->btn_setting,SIGNAL(clicked(bool)),this,SLOT(onSettingBtnClicked(bool)));
    connect(ui->btn_chart,SIGNAL(clicked(bool)),this,SLOT(onChartBtnClicked(bool)));

    connect(setting_page,SIGNAL(sendData(QList<QStringList>)),monitor_page,SLOT(receiveData(QList<QStringList>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect frameRect = rect();
    frameRect.adjust(-1,-1,1,1);
    //绘制背景和边框
//    painter.setPen(QColor(255, 0, 0));
    painter.setBrush(QColor(55, 59, 68));
    painter.drawRoundRect(frameRect,1,1);

    //画navbar
    QPainter painter3(this);
    painter3.setBrush(QColor(53, 57, 66));
    QRect navRect = frameRect;
    navRect.setBottom(navRect.top()+80);
    painter3.drawRect(navRect);
    //画logo

    QPainter painter2(this);
    QRect logoRectTarget(0,0,80,80);
    QRect source(0,0,m_logo.width(),m_logo.height());
    painter2.drawPixmap(logoRectTarget,m_logo,source);

    QPainter painter4(this);
    QRect titleRect(80,0,240,60);
    QFont font("方正正黑简体",14);
    painter4.setPen(QColor(255,255,255));
    painter4.setFont(font);
//    painter4.drawRect(titleRect);
    painter4.drawText(titleRect,Qt::AlignCenter,"泳安视频监控管理系统");
    QPainter painter5(this);
    QRect titleRect2(80,40,240,30);
    QFont font1("Times New Roman",10);
    painter5.setPen(QColor(255,255,255));
    painter5.setFont(font1);
//    painter5.drawRect(titleRect2);
    painter5.drawText(titleRect2,Qt::AlignCenter,"E-eyes Video Managment System");

}

int MainWindow::onMaxClicked(bool checked){
    if(window_status == 0){
        this->showMaximized();
        window_status = 1;
        QPixmap m_icon1;
        m_icon1.load(":/images/max_normal.png");
        QIcon icon1(m_icon1);
        ui->btn_max->setIcon(icon1);
    }
    else if(window_status == 1){
        this->showNormal();
        window_status = 0;
        QPixmap m_icon2;
        m_icon2.load(":/images/max16a.png");
        QIcon icon2(m_icon2);
        ui->btn_max->setIcon(icon2);
    }
    return 0;
}
int MainWindow::onMinClicked(bool checked){
    this->showMinimized();
    return 0;
}
int MainWindow::onCloseClicked(bool checked){
    this->~MainWindow();
    return 0;
}

void MainWindow::updateUI(){
    ui->btn_monitor->style()->unpolish(ui->btn_monitor);
    ui->btn_monitor->style()->polish(ui->btn_monitor);
    ui->btn_video->style()->unpolish(ui->btn_video);
    ui->btn_video->style()->polish(ui->btn_video);
    ui->btn_setting->style()->unpolish(ui->btn_setting);
    ui->btn_setting->style()->polish(ui->btn_setting);
    ui->btn_chart->style()->unpolish(ui->btn_chart);
    ui->btn_chart->style()->polish(ui->btn_chart);
}

void MainWindow::onMoniorClicked(bool checked){
    ui->stackedWidget->setCurrentWidget(monitor_page);
    ui->btn_monitor->setProperty("pressed",true);
    ui->btn_video->setProperty("pressed",false);
    ui->btn_setting->setProperty("pressed",false);
    ui->btn_chart->setProperty("pressed",false);

    updateUI();

}

void MainWindow::onVideoBtnClicked(bool checked){
    ui->stackedWidget->setCurrentWidget(video_page);
    ui->btn_monitor->setProperty("pressed",false);
    ui->btn_video->setProperty("pressed",true);
    ui->btn_setting->setProperty("pressed",false);
    ui->btn_chart->setProperty("pressed",false);

    updateUI();

}
void MainWindow::onSettingBtnClicked(bool checked){
    ui->stackedWidget->setCurrentWidget(setting_page);
    ui->btn_monitor->setProperty("pressed",false);
    ui->btn_video->setProperty("pressed",false);
    ui->btn_setting->setProperty("pressed",true);
    ui->btn_chart->setProperty("pressed",false);

    updateUI();


}

void MainWindow::onChartBtnClicked(bool checked){
    ui->stackedWidget->setCurrentWidget(chart_page);
    ui->btn_chart->setProperty("pressed",true);
    ui->btn_monitor->setProperty("pressed",false);
    ui->btn_video->setProperty("pressed",false);
    ui->btn_setting->setProperty("pressed",false);

    updateUI();

}

