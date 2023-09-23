#include "allmusicinformation.h"
#include<QUrl>
#include<QFileDialog>
#include<QDebug>
#include<QTimer>
#include<QMediaPlayer>
#include<QtSql/QSqlQuery>
#include<QVBoxLayout>
#include<QScrollArea>
#include<QObject>
#include<QDir>
#include<QMediaContent>
#include<QFileInfo>
#include<QString>
#include<QFileInfoList>
#include<QMediaMetaData>
#include<QCoreApplication>


AllMusicInformation::AllMusicInformation()
{
    player=new QMediaPlayer;
    musicList=new QMediaPlaylist();
    player->setVolume(50);   //设置声音大小

    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(getDuration(qint64)));

/*********************歌曲列表***********************/

     QString currentPath=QDir::currentPath();
     QString musicPath =QDir::cleanPath(currentPath + "/../MusicPlayer/music.");
     QString lyricPath =QDir::cleanPath(currentPath + "/../MusicPlayer/musiclyric.");


     //获得所有歌曲名
     //dir1=QDir("D:/QT/MusicPlayer/music");
     dir1=QDir(musicPath);

     QStringList filename;
     filename<<"*.ogg"<<"*.mp3"<<"*.wav";   //添加三个文件类型的过滤器

     QStringList results;   //所有音乐
     results=dir1.entryList(filename,QDir::Files|QDir::Readable,QDir::Name);//获取符合文件类型过滤器条件的文件列表
     musicnumber=results.size();   //音乐总数

     //获取歌曲中所有文件列表，QDir::NoDotAndDotDot：也是一个枚举值，表示不包括 "." 和 ".." 这两个特殊目录
      // musicsList=dir1.entryList(QDir::Files|QDir::NoDotAndDotDot);


/**********全部音乐文件导入播放器*****************/
     QStringList musicFiles=dir1.entryList(QDir::Files);

     foreach(QString musicFile,musicFiles)
     {
        QString musicFilePath=dir1.absoluteFilePath(musicFile);
        musicList->addMedia(QUrl::fromLocalFile(musicFilePath));
     }

     player->setPlaylist(musicList);


/**************所有音乐窗口***************/
     playList=new QListView();
     playList->setWindowTitle("全部音乐目录列表");
     playList->resize(500,600);
     playListModel=new QStringListModel;
     playListModel->setStringList({results});
     playList->setModel(playListModel); //音乐列表模型设置给视图

     QFont font;
     font.setPointSize(13);
     playList->setFont(font);
     QString style="QListView { background-image: url(:/backgroung1.png);}";
     playList->setStyleSheet(style);




/******************获取歌手名*******************/

     QFileInfoList list=dir1.entryInfoList();
     foreach(const QFileInfo& fileInof,list)
     {
         QString filename=fileInof.fileName().trimmed();
         QString artist= this->getArtistFromFileName(filename);  //从文件中读歌手名
         if(artist!="")
         {
           artistSet.insert(artist);
         }
     }

/*****************获得专辑名********************/

     foreach(const QFileInfo& fileInof,list)
     {
         QString filename=fileInof.fileName().trimmed();
         QString album= this->getAlbumFromFileName(filename);  //从文件中读专辑名
         if(album!="")
         {
           albumSet.insert(album);
         }
     }


/*******************歌词*****************/

    //歌词文件导入
    // dir2=QDir("D:\\QT\\MusicPlayer\\musiclyric");
    dir2=QDir(lyricPath);
    lyricsList=dir2.entryList(QDir::Files|QDir::NoDotAndDotDot);   //获取歌词中所有文件列表

}



void AllMusicInformation::MusicPause(bool pause)
{
    if(pause)
    {
        player->pause();
    }
    else
    {
        player->play();
    }
}


void AllMusicInformation::MusicMode(bool singleCycle)
{
    if(singleCycle)
    {
       // musicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); 单曲循环
         musicList->setPlaybackMode(QMediaPlaylist::Loop);
    }
    else
    {
        musicList->setPlaybackMode(QMediaPlaylist::Random);
    }
}


void AllMusicInformation::MusicNoSound(bool nosound)
{
    if(nosound)
    {
      // player->stop();
        player->setVolume(0);
    }
    else
    {
        player->play();
    }

}


void AllMusicInformation::NextMusic()
{


    if(musicList->currentIndex()==musicnumber-1)
    {
      player->playlist()->setCurrentIndex(0);
    }
    else
    {
       musicList->next();
    }

}


void AllMusicInformation::LastMusic()
{

    if(musicList->currentIndex()==0)
    {
        player->playlist()->setCurrentIndex(musicnumber-1);
    }
    else
    {
         musicList->previous();
    }

}


void AllMusicInformation::SetSound(int num)
{
    player->setVolume(num);
}


qint64 AllMusicInformation::getPlayerPosition()
{
    return player->position();
}


qint64 AllMusicInformation::getPlayerDuration()
{
    return allTime;
}

void AllMusicInformation::CurrentMusic(int slidervalue)
{
    player->setPosition(slidervalue);
}


QString AllMusicInformation::getMusicPath()    //得到当前音乐的路径
{
    if(musicList->currentIndex()!=-1)
    {
        QMediaContent mediaContent=musicList->currentMedia();
        QUrl url=mediaContent.request().url();
        QString nowPath=url.toLocalFile();

        return nowPath;
    }
}


QString AllMusicInformation::getMusicName(QString string)//参数为音乐路径
{
    //创建QFileIfo，并设置路径
    QFileInfo musicFileInfo(string);
    QString name=musicFileInfo.fileName();

    return name;
}


QString AllMusicInformation::getMusicLyrics(QString string)  //参数为音乐路径
{
    foreach(QString file,lyricsList)
    {
        QFileInfo fileInfo(file);

        if(fileInfo.baseName()==QFileInfo(string).baseName())
        {
            return dir2.absoluteFilePath(file);  //返回绝对路径
        }

    }

    return "未找到歌词资源";
}


QString AllMusicInformation::getArtistFromFileName( QString& fileName)
{

    int index = fileName.indexOf("-"); // 查找"-"符号的位置
    if (index != -1) {
        QString artist = fileName.left(index).trimmed(); // 提取"-"符号之前的部分，并去除首尾空格
        return artist;
    }

    return QString(); // 如果无法提取到歌手信息，返回空字符串
}


 QString AllMusicInformation::getAlbumFromFileName ( QString& fileName)
 {
     QString processedName = fileName.trimmed(); // 预处理文件名，去除前后的空格
     int firstUnderscoreIndex = processedName.indexOf('-'); // 第一个-的索引
     int secondUnderscoreIndex = processedName.indexOf('-', firstUnderscoreIndex + 1); // 第二个-的索引

     if (firstUnderscoreIndex != -1 && secondUnderscoreIndex != -1 && secondUnderscoreIndex > firstUnderscoreIndex + 1)
      {
         QString album = processedName.mid(firstUnderscoreIndex + 1, secondUnderscoreIndex - firstUnderscoreIndex - 1);
         return album;
      }

       return this->getArtistFromFileName(fileName);  // 如果无法获取专辑名，则返回歌手名
 }


 void AllMusicInformation::getDuration(qint64 a)
 {
     allTime=player->duration();
 }









