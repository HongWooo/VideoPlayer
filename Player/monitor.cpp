#include "monitor.h"
#include "ui_monitor.h"
#include "setting.h"
#include <QString>
#include <qdebug.h>

monitor::monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monitor)
{
    ui->setupUi(this);
}

monitor::~monitor()
{
    delete ui;
}

void monitor::receiveData(QList<QStringList> datalist){
    qDebug()<<"received data:"<<endl;
    for(int i=0;i<datalist.size();i++){
        for(int j=0;j<datalist[i].size();j++){
            qDebug()<<datalist[i][j]<<'\t';
        }
        qDebug()<<endl;
    }
    infoList = datalist;
    initPlayer();
}

void monitor::initPlayer(){
    if(infoList.size() != 0){
        for(int i=0;i<infoList.size();i++){
            QString name = infoList[i][0];
            QString url = infoList[i][1];
            QString state = infoList[i][2];
            url.remove(QChar('\n'), Qt::CaseInsensitive);
            qDebug()<<"url is "<<url<<endl;
            load(url);
            play();
        }
    }
}

bool monitor::load(const QString &url)
{
    if(m_vlcInstance) {
        qDebug()<<"free"<<endl;
        freeVlc();
    }

    if(url.isEmpty()){
        qDebug() << "filepath isEmpty";
        return false;
    }

    bool bload = loadVlc(url.toUtf8().constData());
    if(!bload){
        qDebug() << "bload no isEmpty";
        return false;
    }
    qDebug()<<"load success"<<endl;

    return true;
}

bool monitor::loadVlc(const QString &filepath)
{
    m_vlcInstance = libvlc_new(0, NULL);
    m_media = libvlc_media_new_location(m_vlcInstance, filepath.toUtf8().data());
    if(!m_media){
        freeVlc();
        return m_media;
    }
    m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
    if(!m_mediaPlayer){
        freeVlc();
        return false;
    }
    libvlc_media_parse(m_media);

    libvlc_event_manager_t *em = libvlc_media_player_event_manager(m_mediaPlayer);
    libvlc_event_attach(em, libvlc_MediaPlayerTimeChanged, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerEndReached, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerStopped, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerPlaying, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerPaused, vlcEvents, this);
    //如果把libVLC弹出窗口嵌入到程序中的话，“全屏”功能就不能使用了。
    libvlc_media_player_set_hwnd(m_mediaPlayer, (void *)ui->label_c1->winId());
    return true;
}

void monitor::freeVlc()
{
    if(m_media){
        libvlc_media_release(m_media);
        m_media = Q_NULLPTR;
    }
    if(m_mediaPlayer){
        libvlc_media_player_stop(m_mediaPlayer);
        libvlc_media_player_release(m_mediaPlayer);
        m_mediaPlayer = Q_NULLPTR;
    }
    if(m_vlcInstance){
        libvlc_release(m_vlcInstance);
        m_vlcInstance = Q_NULLPTR;
    }
}

void monitor::play(){
    if(!m_mediaPlayer) return;  //没有加载视频
    if(libvlc_Ended == libvlc_media_get_state(m_media)){
        stop();//vlc这个设定太奇怪了，当自然播放结束，重新播放还得主动调用一次stop();
    }
    libvlc_media_player_play(m_mediaPlayer);
}
void monitor::stop()
{
//    m_bPlaying = false;
    libvlc_media_player_stop(m_mediaPlayer);
}

void monitor::vlcEvents(const libvlc_event_t *ev, void *param)
{
    Q_UNUSED(param);
    switch (ev->type) {
    case libvlc_MediaPlayerTimeChanged:
        break;
    case libvlc_MediaPlayerEndReached://不能在回调里调用stop();否则会卡死
        break;
    case libvlc_MediaPlayerStopped:
        qDebug() << "libvlc_MediaPlayerStopped";
        break;
    case libvlc_MediaPlayerPlaying:
        qDebug() << "libvlc_MediaPlayerPlay";
        break;
    case libvlc_MediaPlayerPaused:
        qDebug() << "libvlc_MediaPlayerPaused";
        break;
    }
}

