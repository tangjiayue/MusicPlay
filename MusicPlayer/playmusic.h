#ifndef PLAYMUSIC_H
#define PLAYMUSIC_H
#include<QWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMediaPlayer>
#include<QGridLayout>
#include<QLabel>
#include<QSlider>
#include<QTimer>
#include<QStackedWidget>
#include<allmusicinformation.h>
#include<chat.h>
#include<QScrollArea>
#include<QMouseEvent>
#include<QTextEdit>
#include<QMovie>
#include<QStringList>
#include<QMap>
#include<QListWidget>
#include<QSplitter>
#include<QTableWidget>
#include<QDialogButtonBox>


class PlayMusic: public QWidget
{
    Q_OBJECT

public:
    PlayMusic(QWidget *parent = nullptr);
    ~PlayMusic();
    void mousePressEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void getLyrics();         //得到文本歌词
    void clickListSong(const QModelIndex& index);    //点击全部音乐列表，播放音乐




private:

    AllMusicInformation *allmusicinformation;
    Chat *chat;

    QPushButton *onOff;      //播放暂停键
    QPushButton *next;       //下一首鍵
    QPushButton *last;       //上一首鍵
    QPushButton *playingMode;      //顺序播放鍵/单曲循环鍵
    QPushButton *returnKey;        //返回鍵
    QPushButton *songs;    //歌曲专辑
    QPushButton *lyric;    //歌词键
    QPushButton *allMusic; //音乐收藏夹
    QPushButton *sound;    //声音
    QPushButton *noSound;  //静音键
    QPushButton *singer;   //按歌手分类
    QPushButton *rollingLyric;  // 滚动歌词
    QPushButton *help;     //帮助
    QPushButton *searchbtn;     //搜索
    QPushButton *random;   //随机点歌

    QSlider *musicSlider;  //音乐进度条
    QSlider *soundSlider;  //音量条

    QLabel* musicImageLabel;   //音乐图片
    QLabel* musicTime;         //音乐播放进度 
    QLabel* songName;          //音乐名字

    QTimer* timer;    //时间计时器
    QPoint z;         //用于界面拖动

/*********************歌词页面************************/

    //滚动歌词
     QWidget *lyricswidget;       //滚动歌词窗
     QMap<int,QString> *lrcMap;   //存储歌词和对应时间
     QMap<int,QString>::iterator iter;

     QLabel* label0;   //滚动歌词显示
     QLabel* label1;
     QLabel* label2;
     QLabel* label3;
     QLabel* label4;
     QLabel* label5;
     QLabel* label6;
     QLabel* label7;
     QLabel* label8;
     QLabel* label9;

     QVBoxLayout* rollinglayout;   //滚动歌词窗布局

     //文本歌词
     QVBoxLayout* layout;       //歌词布局
     QListWidget*lyricwidget;   //歌词文本

/********************歌手页面****************************/

    QWidget* singerWidget;      //歌手页面
    QListWidget *singerList;    //歌手
    QHBoxLayout *singerLayout;  //界面总布局
    QListWidget *songList;      //歌曲

/********************专辑页面****************************/

    QWidget* albumWidget;       //专辑页面
    QListWidget *albumList;     //专辑
    QHBoxLayout *albumLayout;   //界面总布局
    QListWidget *albumSongList; //专辑歌曲

/************************搜索*******************************/

    QWidget *searchWidget;
    QWidget *search;  //搜索框
    QWidget *result;  //搜索结果

    QDialogButtonBox *btnBox ;
    QLineEdit *searchLineEdit;   //搜索框

    QPushButton *searchButton;   //搜索按钮
    QTableWidget *resultTable;   //结果列表

/***************************闹钟**************************/

     QTimer timer1;

     QLineEdit mytime;     // 时间输入

     QPushButton alarm;    //闹钟
     QPushButton onalarm;  //开启闹钟
     QPushButton offalarm; //关闭闹钟

     QLabel tip;           //输入提示词
     QLabel tip1;

/**************************优化布局*******************************/

    QPushButton* first;   //首页的歌曲推荐

    QVBoxLayout* betterLayout;   //优化总布局
    QSplitter *splitterMain;     //总分布窗口
    QStackedWidget* topWidget;   //右上显示窗口



protected slots:
    void showPlayButton();      //播放或暂停
    void showPlayMode();        //播放模式——单曲循环或随机
    void showSoundSlider();     //调整声音
    void showNoSound();         //静音
    void nextMusic();           //下一首音乐
    void laseMusic();           //上一首音乐
    void setSound();            //声音滚动条设置
    void showMusicTime();       //显示当前音乐播放进度
    void currentMusic();        //拉动滚动条后音乐改变
    void showLyricWidget();     //显示文本歌词窗
    void showLyricsWidget();    //显示滚动歌词窗
    void showReturnTip();       //关闭主页面提示
    void showAllMusicList();    //显示全部音乐
    void showSingerWindow();    //显示按歌手分类的播放页面
    void showAlbumWindow();     //显示按专辑分类的播放页面
    void onPositionChanged(qint64 a);     //歌词滚动窗
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);  //曲目播放结束,更新歌词的label
    void showHelpWindow();      //帮助页面
    void Search();              //搜索显示结果
    void playSearchMusic(int row, int column);    //点击搜索到的列表播放音乐
    void searchWindow();        //展示搜索窗

};

#endif // PLAYMUSIC_H
