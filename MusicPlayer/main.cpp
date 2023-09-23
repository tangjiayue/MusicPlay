#include "playmusic.h"
#include<allmusicinformation.h>
#include<QPixmap>
#include<QSplashScreen>
#include<QApplication>
#include<chat.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/new/prefix2/music.png");
    pixmap.scaled(50,50);
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();  //使程序启动画面时同时能响应鼠标其他事件

    PlayMusic w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
