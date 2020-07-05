#include "video.h"
#include "ui_video.h"
#include <qdebug.h>
#include <QFile>



Video::Video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);


    m_fileDialog = new QFileDialog(this);
    m_fileDialog->setWindowTitle(tr("选择视频"));
    m_fileDialog->setDirectory(".");
    m_fileDialog->setNameFilter(tr("Video Files(*.mp4 *.mp3 *flv *3gp *rmvb)"));
    m_fileDialog->hide();

    holder = ui->label_video_holder;
    holder->installEventFilter(this);

    connect(ui->btn_open,SIGNAL(clicked(bool)),this,SLOT(onBtnOpenClicked()));
    connect(ui->btn_pause,SIGNAL(clicked(bool)),this,SLOT(onBtnPauseClicked()));
    connect(ui->slide_bar,SIGNAL(sliderReleased()),this,SLOT(onSlideBarReleased()));
    connect(ui->btn_next,SIGNAL(clicked(bool)),this,SLOT(onBtnNextClicked()));
    connect(ui->btn_pre,SIGNAL(clicked(bool)),this,SLOT(onBtnPreClicked()));
    connect(ui->btn_replay,SIGNAL(clicked(bool)),this,SLOT(onBtnReplayClicked()));
}

Video::~Video()
{
    delete ui;
}

void Video::vlcEvents(const libvlc_event_t *ev, void *param)
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

bool Video::load(const QString &filepath)
{
    if(m_vlcInstance) {
        qDebug()<<"free"<<endl;
        freeVlc();
    }

    if(filepath.isEmpty()){
        qDebug() << "filepath isEmpty";
        return false;
    }
    if(!QFile::exists(filepath)){
        qDebug() << "QFile no exists";
        return false;
    }

    bool bload = loadVlc(QDir::toNativeSeparators(filepath).toUtf8().constData());

    if(!bload){
        qDebug() << "bload no isEmpty";
        return false;
    }
    qDebug()<<"load success"<<endl;
    //初始化slider_bar
    slider = ui->slide_bar;
    slider->installEventFilter(this);
    //初始化video hoder (label)，设置监听事件

    play();

    return true;
}

bool Video::loadVlc(const QString &filepath)
{
    qDebug()<<"file path is "<<filepath<<endl;
    m_vlcInstance = libvlc_new(0, NULL);
    m_media = libvlc_media_new_path(m_vlcInstance, filepath.toUtf8().data());
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
    libvlc_media_player_set_hwnd(m_mediaPlayer, (void *)ui->label_video_holder->winId());
    return true;
}

void Video::play()
{
    if(!m_mediaPlayer) return;  //没有加载视频
    m_timerid = startTimer(500);    //开启一个计时器
    if(libvlc_Ended == libvlc_media_get_state(m_media)){
        stop();//vlc这个设定太奇怪了，当自然播放结束，重新播放还得主动调用一次stop();
    }
    libvlc_media_player_play(m_mediaPlayer);

    m_bPlaying = true;
    QPixmap m_icon_pause;
    m_icon_pause.load(":/images/pause64.png");
    QIcon icon_pause(m_icon_pause);
    ui->btn_pause->setIcon(icon_pause);
}

void Video::pause()
{
    if((bool)libvlc_media_player_is_playing(m_mediaPlayer)){
        m_bPlaying = false;
        libvlc_media_player_set_pause(m_mediaPlayer, 1);//暂停
        QPixmap m_icon_begin;
        m_icon_begin.load(":/images/begin64.png");
        QIcon icon_begin(m_icon_begin);
        ui->btn_pause->setIcon(icon_begin);
    }
    killTimer(m_timerid);
}

void Video::stop()
{
    m_bPlaying = false;
    libvlc_media_player_stop(m_mediaPlayer);
}


void Video::onBtnOpenClicked()
{

    m_fileDialog->show();
    if(m_fileDialog->exec() == QDialog::Accepted) {
        QString path = m_fileDialog->selectedFiles()[0];
        load(path);
    }
}

void Video::onBtnPauseClicked(){
    if(m_bPlaying)
        pause();
    else
        play();

}

void Video::freeVlc()
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

void Video::onSlideBarReleased(){
    if(m_mediaPlayer){
        int dur = slider->maximum() - slider->minimum();
        float posf = slider->value()*1.0/dur;
        libvlc_media_player_set_position(m_mediaPlayer,posf);
    }
}

void Video::onBtnNextClicked(){
    if(m_mediaPlayer){
        int step = slider->singleStep()*5;
        int dur = slider->maximum() - slider->minimum();
        float posf = (slider->value()+step)*1.0/dur;
        if(posf <= 1)
        libvlc_media_player_set_position(m_mediaPlayer,posf);
    }
}

void Video::onBtnPreClicked(){
    if(m_mediaPlayer){
        int step = slider->singleStep()*5;
        int dur = slider->maximum() - slider->minimum();
        float posf = (slider->value()-step)*1.0/dur;
        if(posf>=0)
            libvlc_media_player_set_position(m_mediaPlayer,posf);
    }
}

void Video::onBtnReplayClicked(){
    if(m_mediaPlayer){
        libvlc_media_player_set_position(m_mediaPlayer,0);
        play();
    }
}
bool Video::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==slider)
    {
        if (event->type()==QEvent::MouseButtonPress)           //判断类型，鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) //判断左键
            {
               int dur = slider->maximum() - slider->minimum();
               int pos = slider->minimum() + dur * ((double)mouseEvent->x() / slider->width());
               qDebug()<<"dur="<<dur<<"pos="<<pos<<endl;
               if(pos != slider->sliderPosition())
                {
                  slider->setValue(pos);
                  float posf = pos*1.0/dur;
                  libvlc_media_player_set_position(m_mediaPlayer,posf);
                }
            }
        }
    }
    if(obj==holder){
        if (event->type()==QEvent::MouseButtonPress)           //判断类型，鼠标点击
        {
            qDebug()<<"点击屏幕"<<endl;
        }
    }

    return QObject::eventFilter(obj,event);
}

void Video::timerEvent(QTimerEvent *event){
    int curtime;
    int duration;
    int tns, thh, tmm, tss;
    float progress;
    QString curtimestr,durationstr;

    curtime = libvlc_media_player_get_time(m_mediaPlayer);
    if(curtime != 0){
        //change to second
        tns = curtime/1000;
        thh  = tns / 3600;
        tmm  = (tns % 3600) / 60;
        tss  = (tns % 60);
        curtimestr.sprintf("%02d:%02d:%02d",thh,tmm,tss);
        ui->label_curtime->setText(curtimestr);
    }
    duration  = libvlc_media_player_get_length(m_mediaPlayer);
    if(duration != 0){
        tns = duration/1000;
        thh  = tns / 3600;
        tmm  = (tns % 3600) / 60;
        tss  = (tns % 60);
        durationstr.sprintf("%02d:%02d:%02d",thh,tmm,tss);
        ui->label_endtime->setText(durationstr);
        progress=curtime*1.0/duration;
        int dur = slider->maximum() - slider->minimum();
        int pos = slider->minimum() + dur * progress;
        slider->setValue(pos);
    }
}

void Video::keyPressEvent(QKeyEvent *ev){
    qDebug()<<"key="<<ev->key()<<endl;

    if(ev->key()==16777220){
        qDebug()<<"enter"<<endl;
    //按下enter键，全屏
        if(isfullscreen){
            holder->setWindowFlags(Qt::SubWindow);
            holder->showNormal();
            isfullscreen = false;
        }
        else{
            holder->setWindowFlags(Qt::Dialog);
            holder->showFullScreen();
            isfullscreen = true;
        }
    }
}


