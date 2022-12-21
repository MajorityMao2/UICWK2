//
// Created by twak on 11/11/2019.
//

#include "the_player.h"


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
void ThePlayer::setVoice(int v){
    setVolume(v);
}
void ThePlayer::setFaster(){

    qint64 position = this->position();
    position = position + 2000;

    this->setPosition(position);

}
void ThePlayer::setSlower(){
    qint64 position = this->position();
    position = position - 2000;

    this->setPosition(position);
}

void ThePlayer::playStop(){
    if (statement){
        pause();

    }
    else{
        play();
    }
    statement = !statement;
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    //pushButton_play_and_pause->setEnabled(true);

    play();
}


