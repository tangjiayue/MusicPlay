#ifndef ALLMUSICINFORMATION_H
#define ALLMUSICINFORMATION_H

#include<QDialog>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QListWidget>
#include<QtSql/QSqlDatabase>
#include<QMap>
#include<QListView>
#include<QStringListModel>
#include<QDir>
#include<QSet>

class AllMusicInformation:public QDialog
{
    Q_OBJECT

public:

    AllMusicInformation();
    void MusicPause(bool pause);          //控制音乐暂停
    void MusicMode(bool singleCycle);     //音乐播放模式
    void MusicNoSound(bool stop);            //静音播放音乐
    void NextMusic();
    void LastMusic();
    void SetSound(int num);               //声音设置
    qint64 getPlayerPosition();             //音乐进度
    qint64 getPlayerDuration();             //音乐时长
    void CurrentMusic(int slidervalue);     //滚动条改变后的音乐
    QString getMusicPath();         //获取当前音乐的路径
    QString getMusicName(QString string);  //获得当前音乐名
    QString getMusicLyrics(QString string);  //获得当前歌词路径
    QString getArtistFromFileName( QString& fileName); //从文件名获得歌手
    QString getAlbumFromFileName ( QString& fileName);


    int musicnumber;  // 音乐总数

    QListView* playList;   //歌曲列表
    QStringListModel* playListModel;

    QDir dir1;   //歌曲
    QDir dir2;   //歌词

    QStringList lyricsList;   //歌词文件
    //QStringList musicsList;   //歌曲文件

    QSet<QString> artistSet;    //集合存储歌手名，确保唯一
    QSet<QString> albumSet;     //集合存储歌手名，确保唯一

    QMediaPlayer *player;       //音乐播放器
    QMediaPlaylist* musicList;  //音乐列表


private:   
     qint64 allTime;        //音乐总时长


public slots:
      void getDuration(qint64 a);


};

#endif // ALLMUSICINFORMATION_H
