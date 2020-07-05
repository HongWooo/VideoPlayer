#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include "vlc/vlc.h"
#include <QFileDialog>
#include <QTimer>
#include <QMutex>
#include <QSlider>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include <QRect>

namespace Ui {
class Video;
}

struct libvlc_media_track_info_t;
struct libvlc_media_t;
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class Video : public QWidget
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = 0);
    ~Video();
    bool load(const QString &filepath);
    void play();
    void pause();
    void stop();
    bool loadVlc(const QString &filepath);
    void freeVlc();

protected:
    static void vlcEvents(const libvlc_event_t *ev, void *param);
    bool eventFilter(QObject *obj, QEvent *event);  //事件过滤器
    void timerEvent(QTimerEvent *event);    //计时器
    void keyPressEvent(QKeyEvent *ev);  //键盘监听事件
//    void mousePressEvent(QMouseEvent *event);   //鼠标点击事件
//    void paintEvent(QPaintEvent *event);    //绘制

private slots:
    void onBtnPauseClicked();  //暂停
    void onBtnNextClicked();   //快进
    void onBtnPreClicked();    //回退
    void onBtnOpenClicked();    //加载并播放
    void onSlideBarReleased();  //进度条松开
    void onBtnReplayClicked();  //重新播放
private:
    Ui::Video *ui;

    libvlc_instance_t *m_vlcInstance = Q_NULLPTR;   //一个libvlc的实例
    libvlc_media_t *m_media = Q_NULLPTR;    //一个可播放的媒体的抽象表达.它包含了这个媒体的位置以及各种可选的元数据.
    libvlc_media_player_t *m_mediaPlayer = Q_NULLPTR;   //一个空的媒体播放器对象

    QFileDialog *m_fileDialog = Q_NULLPTR;
    QTimer *m_timer = Q_NULLPTR;
    QSlider *slider;
    QLabel *holder;
    QMutex m_mutex;
    qint64 m_duration = 0;
    bool m_bPlaying = false;
    bool isfullscreen = false;  //判断全屏

    int m_timerid;
};

#endif // VIDEO_H
