#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QPixmap>
#include "monitor.h"
#include "video.h"
#include "setting.h"
#include "chart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    ~MainWindow();
    void updateUI();

private slots:
    int onMaxClicked(bool checked);
    int onMinClicked(bool checked);
    int onCloseClicked(bool checked);
    void onMoniorClicked(bool checked);
    void onVideoBtnClicked(bool checked);
    void onSettingBtnClicked(bool checked);
    void onChartBtnClicked(bool checked);

private:
    Ui::MainWindow *ui;
    QPixmap m_logo;
    int window_status = 1;

    monitor *monitor_page;  //视频监控窗口
    Video *video_page;  //视频回放窗口
    Setting *setting_page;  //设置界面
    Chart *chart_page;   //实时曲线界面
};

#endif // MAINWINDOW_H
