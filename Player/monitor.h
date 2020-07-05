#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <QList>
#include <QStringList>
#include <QString>
#include "vlc/vlc.h"

struct libvlc_media_track_info_t;
struct libvlc_media_t;
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

namespace Ui {
class monitor;
}

class monitor : public QWidget
{
    Q_OBJECT

public:
    explicit monitor(QWidget *parent = 0);
    ~monitor();
    bool loadVlc(const QString &filepath);
    void freeVlc();
    bool load(const QString &filepath);
    void initPlayer();
    void play();
    void stop();

protected:
    static void vlcEvents(const libvlc_event_t *ev, void *param);
private slots:
    void receiveData(QList<QStringList>);

private:
    Ui::monitor *ui;
    QList<QStringList> infoList;
    //暂时只支持播放一个视频
    libvlc_instance_t *m_vlcInstance = Q_NULLPTR;   //一个libvlc的实例
    libvlc_media_t *m_media = Q_NULLPTR;    //一个可播放的媒体的抽象表达.它包含了这个媒体的位置以及各种可选的元数据.
    libvlc_media_player_t *m_mediaPlayer = Q_NULLPTR;   //一个空的媒体播放器对象
};

#endif // MONITOR_H
