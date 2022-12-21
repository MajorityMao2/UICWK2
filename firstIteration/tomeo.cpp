//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include "QSlider"
// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::information(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }
    // stop,continue,forward,backward
    // a row of button to stop, faster and slower
    QWidget *buttonwidget2 = new QWidget();
    // a list of the buttons
    std::vector<QPushButton*> buttons2;
    // the buttons are arranged horizontally
    QHBoxLayout *layout2 = new QHBoxLayout();
    buttonwidget2->setLayout(layout2);
    for(int i=0; i<3;i++){
        QPushButton *button2 = new QPushButton(buttonwidget2);
        buttons2.push_back(button2);
        layout2->addWidget(button2);
        if (i==0)
        {button2->setText("backward 2s");
        button2->connect(button2,&QPushButton::clicked,player,&ThePlayer::setSlower);
        }
        else if(i==1){
            button2->setText("stop");
            button2->connect(button2,&QPushButton::clicked,player,&ThePlayer::playStop);
        }
        else if (i==2){
            button2->setText("forward 2s");

            button2->connect(button2,&QPushButton::clicked,player,&ThePlayer::setFaster);
        }

        button2->resize(200,110);
    }

    // set voice
    int nMin = 0;
    int nMax = 100;
    int nSingleStep = 1;
    QSlider *pSlider = new QSlider;
    pSlider->setOrientation(Qt::Horizontal);  // 水平方向
    pSlider->setMinimum(nMin);  // 最小值
    pSlider->setMaximum(nMax);  // 最大值
    pSlider->setSingleStep(nSingleStep);

    pSlider->connect(pSlider,&QSlider::sliderMoved,player,[=](){player->setVolume(pSlider->value());});


    layout2->addWidget(pSlider);



    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);


    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonwidget2);
    top->addWidget(buttonWidget);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
