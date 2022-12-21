//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

extern double endPresent;
extern QTimer* timer;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}


void ThePlayer::jumpTo (TheButtonInfo* button) {
//    timer->stop();
    setMedia( * button -> url);
    play();
//    timer->start(20);
}


//快进
void ThePlayer::seekUp()
{
    qDebug() << endPresent;

    timer->stop();
    double end = position() + 1000;

    if(end > duration()){
        endPresent = 100;
    }
    else{
        endPresent = 100 * end / duration();
    }
    setPosition(end);
    timer->start(20);

    /*
    position();      //可以获取当前播放位置，单位均为毫秒
    setPosition();   //跳转到一个播放点,通过关联positionChanged()信号可以随时获得播放进度
    duration();      //获得当前媒体的时长，单位为毫秒
    */


    //获取时间
//    video_length = duration();
//    video_pos    = position();

}

double ThePlayer::getCurrentPresent()
{
    endPresent = 100 * position()/duration();
    return endPresent;
}
