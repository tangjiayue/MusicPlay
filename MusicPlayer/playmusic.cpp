#include "playmusic.h"
#include "allmusicinformation.h"
#include<windows.h>
#include<QMessageBox>
#include<QPixmap>
#include<QStackedWidget>
#include<QFrame>
#include<QWidget>
#include<QBitmap>
#include<QDockWidget>
#include<QMediaPlayer>
#include<QFile>
#include<QTimer>
#include<QString>
#include<QPalette>
#include<QMatrix>
#include<QStyleOption>
#include<QPainter>
#include<QBitmap>
#include<QStackedWidget>
#include<QGraphicsDropShadowEffect>
#include<QHeaderView>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include<QRandomGenerator>


PlayMusic::PlayMusic(QWidget *parent)
     : QWidget(parent)
{

/***************************主界面美化*******************************/

    allmusicinformation=new AllMusicInformation();
    chat=new Chat();

    //主界面美化
    setWindowTitle(tr("*****音乐播放器*****"));
    setFixedSize(1200,800);
    Sleep(1000);
    this->move(500,100);

    QPalette palette(this->palette());
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/new/prefix2/mainBackground.jpg")));
    this->setPalette(palette);

    //圆角
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),20,20);
    setMask(bmp);
    p.end();


/**********退出（返回）键，专辑键，歌手键，文本歌词键，滚动歌词键，帮助键，随机点播键**************/

   //按钮
   returnKey=new QPushButton(this);   //返回鍵
   songs=new QPushButton(this);       //歌曲专辑
   lyric=new QPushButton(this);       //文本歌词
   singer=new QPushButton(this);      //歌手
   rollingLyric=new QPushButton(this);  //滚动歌词
   help=new QPushButton(this);        //帮助
   random=new QPushButton(this);      //随机点播

   //按钮美化
   QPixmap icon1(":/back.jpg");
   returnKey->setIcon(QIcon(icon1));
   returnKey->setIconSize(QSize(40,40));

   songs->setText("专辑");
   songs->resize(40,40);

   QFont songsFont;
   songsFont.setPixelSize(20);
   songsFont.setBold(true);
   songs->setFont(songsFont);

   lyric->setText("文本歌词");
   lyric->resize(40,40);
   lyric->setFont(songsFont);

   rollingLyric->setText("滚动歌词");
   rollingLyric->resize(40,40);
   rollingLyric->setFont(songsFont);

   singer->setText("歌手");
   singer->resize(40,40);
   singer->setFont(songsFont);

   help->setText("帮助");
   help->resize(40,40);
   help->setFont(songsFont);

   random->setText("随机点播");
   random->resize(40,40);
   random->setFont(songsFont);

   //点击随机按钮
   connect(random,&QPushButton::clicked,[=](){

       int r=QRandomGenerator::global()->bounded(0,allmusicinformation->musicnumber);

       allmusicinformation->player->playlist()->setCurrentIndex(r);
       allmusicinformation->player->play();

       onOff->setIcon(QIcon(":/new/prefix2/on1.png"));
       onOff->setFlat(true);

       allmusicinformation->MusicPause(false);

       topWidget->setCurrentIndex(0);

           });


/*********************当前播放歌曲名label显示**********************/

   songName=new QLabel(this);
   songName->setFont(songsFont);  //songsFont在上面专辑按钮定义过

   //用于更新音乐时间等
   timer=new QTimer();
   timer->setInterval(1000);     //间隔1秒
   timer->start();

   musicTime=new QLabel();       //音乐播放时间
   musicTime->setText("00:00");

   musicSlider=new QSlider(Qt::Horizontal,this);     //音乐进度条
   musicSlider->setTracking(true);
   musicSlider->setRange(0,1000);


/************播放键，上一首，下一首，播放模式键，所有音乐键，声音键*************/

    //按钮
    onOff=new QPushButton(this);      //播放键
    next=new QPushButton(this);       //下一首鍵
    last=new QPushButton(this);       //上一首鍵
    playingMode=new QPushButton(this);      //单曲循环鍵/随机播放鍵
    sound=new QPushButton(this);      //声音大小
    allMusic=new QPushButton(this);   //音乐文件夹


    /****关于声音调节****/
    noSound=new QPushButton(this);
    noSound->close();

    soundSlider=new QSlider(Qt::Horizontal,this);  //声音滚动条
    soundSlider->setTracking(true);
    soundSlider->setRange(0,100);
    soundSlider->setValue(50);
    soundSlider->close();

   //按钮图片，风格
    QPixmap icon2(":/new/prefix2/off1.png");
    onOff->setIcon(QIcon(icon2));
    onOff->setIconSize(QSize(60,60));

    QPixmap icon3(":/new/prefix2/next1.png");
    next->setIcon(QIcon(icon3));
    next->setIconSize(QSize(40,40));

    QPixmap icon4(":/new/prefix2/last1.png");
    last->setIcon(QIcon(icon4));
    last->setIconSize(QSize(40,40));

    QPixmap icon5(":/new/prefix2/shuffle1.png");
    playingMode->setIcon(QIcon(icon5));
    playingMode->setIconSize(QSize(40,40)); //最开始为顺序播放

    QPixmap icon6(":/new/prefix2/sound.jpg");
    sound->setIcon(QIcon(icon6));
    sound->setIconSize(QSize(40,40));

    QPixmap icon7(":/new/prefix2/allMusic.png");
    allMusic->setIcon(QIcon(icon7));
    allMusic->setIconSize(QSize(40,40));

    QPixmap icon8(":/new/prefix2/yesSound.jpg");
    noSound->setIcon(QIcon(icon8));
    noSound->setIconSize(QSize(40,40));

    //播放暂停
    connect(onOff,SIGNAL(clicked()),this,SLOT(showPlayButton()));
    //随机播放、单曲循环
    connect(playingMode,SIGNAL(clicked()),this,SLOT(showPlayMode()));
    //显示声音调节
    connect(sound,SIGNAL(clicked()),this,SLOT(showSoundSlider()));
    //声音静音
    connect(noSound,SIGNAL(clicked()),this,SLOT(showNoSound()));
    //下一首
    connect(next,SIGNAL(clicked()),this,SLOT(nextMusic()));
    //上一首
    connect(last,SIGNAL(clicked()),this,SLOT(laseMusic()));
    //滚动条调声音
    connect(soundSlider,SIGNAL(valueChanged(int)),this,SLOT(setSound()));
    //显示音乐播放时间，更新时间，歌词接收
    connect(timer,SIGNAL(timeout()),this,SLOT(showMusicTime()));
    //滚动条调音乐进度
    connect(musicSlider,SIGNAL(sliderMoved(int)),this,SLOT(currentMusic()));
    //界面关闭
    connect(returnKey,SIGNAL(clicked()),this,SLOT(showReturnTip()));
    //显示文本歌词窗面
    connect(lyric,SIGNAL(clicked()),this,SLOT(showLyricWidget()));
    //显示所有音乐列表
    connect(allMusic,SIGNAL(clicked()),this,SLOT(showAllMusicList()));
    //点击所有音乐的列表播放音乐！！！
    connect(allmusicinformation->playList,&QListView::clicked,this,&PlayMusic::clickListSong);
    //显示滚动歌词窗面
    connect(rollingLyric,SIGNAL(clicked()),this,SLOT(showLyricsWidget()));
    //帮助页面的显示
    connect(help,SIGNAL(clicked()),this,SLOT(showHelpWindow()));


  /**********************歌词************************/

    //滚动歌词
    lrcMap=new QMap<int,QString>;
    lyricswidget=new QWidget();
    lyricswidget->setFixedSize(600,900);
    lyricswidget->move(500,100);

    label0=new QLabel(this);
    label1=new QLabel(this);
    label2=new QLabel(this);
    label3=new QLabel(this);
    label4=new QLabel(this);
    label5=new QLabel(this);
    label6=new QLabel(this);
    label7=new QLabel(this);
    label8=new QLabel(this);
    label9=new QLabel(this);

    QGraphicsDropShadowEffect *effect1 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect();

    effect1->setColor(Qt::white);
    effect1->setBlurRadius(8);
    effect1->setOffset(0);
    effect2->setColor(Qt::white);
    effect2->setBlurRadius(8);
    effect2->setOffset(0);


    QFont labelFont;
    labelFont.setPixelSize(20);
    labelFont.setBold(true);

    label0->setFont(labelFont);
    label1->setFont(labelFont);
    label2->setFont(labelFont);
    label3->setFont(labelFont);
    label4->setFont(labelFont);
    label5->setFont(labelFont);
    label6->setFont(labelFont);
    label7->setFont(labelFont);
    label8->setFont(labelFont);
    label9->setFont(labelFont);

    label4->setGraphicsEffect(effect1);
    label4->setStyleSheet("color:#c7679f");
    label5->setGraphicsEffect(effect2);
    label5->setStyleSheet("color:#c7679f");


    rollinglayout=new QVBoxLayout(lyricswidget);
    rollinglayout->addWidget(label0);
    rollinglayout->addWidget(label1);
    rollinglayout->addWidget(label2);
    rollinglayout->addWidget(label3);
    rollinglayout->addWidget(label4);
    rollinglayout->addWidget(label5);
    rollinglayout->addWidget(label6);
    rollinglayout->addWidget(label7);
    rollinglayout->addWidget(label8);
    rollinglayout->addWidget(label9);

    //滚动歌词更新
    connect(allmusicinformation->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
    //label清空，文本歌词更新
    connect(allmusicinformation->player,&QMediaPlayer::mediaStatusChanged, this, &PlayMusic::mediaStatusChanged);

    //文本歌词
    lyricwidget=new QListWidget;
    lyricwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // 根据需要显示垂直滚动条
    lyricwidget->setWindowFlags(Qt::FramelessWindowHint);

    //文本歌词背景
    QFont font;
    font.setPointSize(13);
    lyricwidget->setFont(font);
    QString style="QListView { background-image: url(:/backgroung1.png);}";
    lyricwidget->setStyleSheet(style);
    lyricwidget->setMask(bmp);

/*******************************专辑*********************************/

    albumWidget=new QWidget;
    albumList=new QListWidget();
    albumSongList=new QListWidget();

    // 设置字体
    QFont fontlist("Arial", 12);
    albumList->setFont(fontlist);
    albumSongList->setFont(fontlist);

    // 设置背景颜色
    albumList->setStyleSheet("background-image: url(:/new/prefix2/mainBackground.jpg);");
    albumSongList->setStyleSheet("background-image: url(:/new/prefix2/mainBackground.jpg);");

    //布局，左边专辑名，右边歌曲名
    albumLayout=new QHBoxLayout(albumWidget);
    albumLayout->addWidget(albumList,1);
    albumLayout->addWidget(albumSongList,2);


    //专辑录入到列表
    foreach(const QString& album,allmusicinformation->albumSet)
    {
        QListWidgetItem* item=new QListWidgetItem(album);

        item->setData(Qt::UserRole, album); // 设置用户数据为专辑名
        albumList->addItem(item);

    }

    //专辑点击，信号接收，出现歌名
    connect(albumList,&QListWidget::itemClicked,[=](QListWidgetItem* item){

        QString album = item->data(Qt::UserRole).toString();
        albumSongList->clear();

        //由专辑名获得该专辑的所有歌曲文件
        QFileInfoList fileList=allmusicinformation->dir1.entryInfoList();
        foreach (const QFileInfo& fileInfo, fileList)
        {

            QString fileName = fileInfo.fileName();
            QString currentAlbum =allmusicinformation->getAlbumFromFileName(fileName);

            if(currentAlbum==album)
            {
               QListWidgetItem* albumSongItem =new QListWidgetItem(fileName);
               albumSongItem->setData(Qt::UserRole, fileInfo.filePath()); // 设置用户数据为文件路径
               albumSongList->addItem(albumSongItem);

            }
        }
    });


   //点击专辑里面的音乐开始播放
     connect(albumSongList,&QListWidget::itemClicked,this,[=](){

        //获得当前选中的列表项
         QListWidgetItem* selectedItem = albumSongList->currentItem();

         QString filePath=selectedItem->data(Qt::UserRole).toString();
         int songIndex = -1; // 初始化索引为-1，表示未找到

         // 遍历播放列表
         for (int i = 0; i < allmusicinformation->player->playlist()->mediaCount(); ++i)
         {
             QMediaContent media = allmusicinformation->player->playlist()->media(i);
             if (media.canonicalUrl().toLocalFile() == filePath)
             {
                 // 找到了匹配的歌曲，记录索引，并跳出循环
                 songIndex = i;
                 break;
             }
         }
         allmusicinformation->player->playlist()->setCurrentIndex(songIndex);
         allmusicinformation->player->play();

         onOff->setIcon(QIcon(":/new/prefix2/on1.png"));
         onOff->setFlat(true);

         allmusicinformation->MusicPause(false);

           });

   //点击专辑按钮，出现专辑页面
    connect(songs,SIGNAL(clicked()),this,SLOT(showAlbumWindow()));

/*****************************歌手***************************/

    singerWidget=new QWidget;
    singerList=new QListWidget();
    songList=new QListWidget();

    // 设置字体
    QFont fontslist("Arial", 12);
    singerList->setFont(fontslist);
    songList->setFont(fontslist);

    // 设置背景颜色
    singerList->setStyleSheet("background-image: url(:/new/prefix2/mainBackground.jpg);");
    songList->setStyleSheet("background-image: url(:/new/prefix2/mainBackground.jpg);");

    singerLayout=new QHBoxLayout(singerWidget);
    singerLayout->addWidget(singerList,1);
    singerLayout->addWidget(songList,2);

    //歌手录入到列表
    foreach(const QString& artist,allmusicinformation->artistSet)
    {
        QListWidgetItem* item=new QListWidgetItem(artist);
        item->setData(Qt::UserRole, artist); // 设置用户数据为歌手名

        singerList->addItem(item);

    }

    //歌手点击，信号接收，出现歌名
    connect(singerList,&QListWidget::itemClicked,[=](QListWidgetItem* item){

        QString artist = item->data(Qt::UserRole).toString();
        songList->clear();

        //由歌手名获得该歌手的所有歌曲文件
        QFileInfoList fileList=allmusicinformation->dir1.entryInfoList();
        foreach (const QFileInfo& fileInfo, fileList)
        {
            QString fileName = fileInfo.fileName();
            QString currentArtist =allmusicinformation->getArtistFromFileName(fileName);

            if(currentArtist==artist)
            {
               QListWidgetItem* songItem =new QListWidgetItem(fileName);
               songItem->setData(Qt::UserRole, fileInfo.filePath()); // 设置用户数据为文件路径
               songList->addItem(songItem);

            }
        }
    });


   //点击歌手里面的音乐开始播放
     connect(songList,&QListWidget::itemClicked,this,[=](){

        //获得当前选中的列表项
         QListWidgetItem* selectedItem = songList->currentItem();

         QString filePath=selectedItem->data(Qt::UserRole).toString();
         int songIndex = -1; //初始化索引为-1，表示未找到

         //遍历播放列表
         for (int i = 0; i < allmusicinformation->player->playlist()->mediaCount(); ++i)
         {
             QMediaContent media = allmusicinformation->player->playlist()->media(i);
             if (media.canonicalUrl().toLocalFile() == filePath)
             {
                 //找到了匹配的歌曲，记录索引，并跳出循环
                 songIndex = i;
                 break;
             }
         }

       allmusicinformation->player->playlist()->setCurrentIndex(songIndex);
       allmusicinformation->player->play();

       onOff->setIcon(QIcon(":/new/prefix2/on1.png"));
       onOff->setFlat(true);

       allmusicinformation->MusicPause(false);

           });

      connect(singer,SIGNAL(clicked()),this,SLOT(showSingerWindow()));       //歌手列表出现

/*****************************搜索********************************/

      searchbtn=new QPushButton;
      searchbtn->setText("搜索");
      searchbtn->resize(40,40);
      searchbtn->setFont(songsFont);
      connect(searchbtn,SIGNAL(clicked()),this,SLOT(searchWindow()));   //搜索窗口切换

      searchWidget=new QWidget;
      btnBox =new QDialogButtonBox(Qt::Vertical);
      result=new QWidget(searchWidget);   //搜索结果
      search=new QWidget(searchWidget);   //搜索输入


      //圆角
      QBitmap bmps(searchWidget->size());
      bmps.fill();
      QPainter ps(&bmp);
      ps.setPen(Qt::NoPen);
      ps.setBrush(Qt::black);
      ps.drawRoundedRect(bmps.rect(),20,20);
      setMask(bmps);
      ps.end();

      searchLineEdit = new QLineEdit(search);             //搜索输入框
      searchButton = new QPushButton("Search", search);   //搜索按钮
      QPixmap iconb(":/search.jpg");
      searchButton->setIcon(QIcon(iconb));


      //搜索结果表
      resultTable = new QTableWidget(result);
      resultTable->setColumnCount(2);
      resultTable->setHorizontalHeaderLabels(QStringList() << "Music Name" << "Path");
      resultTable->resize(600,300);
      resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

      btnBox->addButton(searchButton,QDialogButtonBox::ActionRole);


      //搜索布局
      QHBoxLayout *searchlayout=new QHBoxLayout(search);
      searchlayout->addWidget(searchButton,1);
      searchlayout->addWidget(searchLineEdit,4);


      //结果布局
      QVBoxLayout *searchLayout = new QVBoxLayout(result);
      searchLayout->addWidget(resultTable);


      //搜索框总部布局
      QVBoxLayout *allsearchLayout = new QVBoxLayout;
      allsearchLayout->addWidget(search);
      allsearchLayout->addWidget(result);

      searchWidget->setLayout(allsearchLayout);
      searchWidget->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
      result->hide();
      searchWidget->show();


      //连接搜索按钮的点击信号与搜索槽函数
      connect(searchButton, &QPushButton::clicked, this, &PlayMusic::Search);

      //回车发送搜索
      connect(searchLineEdit, &QLineEdit::returnPressed, this, &PlayMusic::Search);

      //连接结果列表的双击事件与播放音乐槽函数
      connect(resultTable, &QTableWidget::cellDoubleClicked, this, &PlayMusic::playSearchMusic);

/********************闹钟**********************/

      QWidget* alarmWidget=new QWidget;
      timer1.setSingleShot(true);  //单次触发
      alarm.setText("闹钟");      
      tip.setText("请输入想要设置的闹钟时间(整数)：");
      tip.adjustSize();
      tip1.setText("分钟");
      onalarm.setText("开启闹钟");
      offalarm.setText("关闭闹钟");

      alarm.resize(40,40);
      alarm.setFont(songsFont);
      tip.resize(40,40);
      tip.setFont(songsFont);
      tip1.resize(40,40);
      tip1.setFont(songsFont);
      onalarm.resize(40,40);
      onalarm.setFont(songsFont);
      offalarm.resize(40,40);
      offalarm.setFont(songsFont);
      mytime.setFont(songsFont);

      //布局
      QHBoxLayout* timeLayout=new QHBoxLayout;
      timeLayout->addWidget(&tip);
      timeLayout->addWidget(&mytime);
      timeLayout->addWidget(&tip1);
      timeLayout->addStretch();
      QHBoxLayout* timeLayout1=new QHBoxLayout;
      timeLayout1->addWidget(&onalarm);
      timeLayout1->addWidget(&offalarm);
      timeLayout1->addStretch();

      QVBoxLayout* timeLayout2=new QVBoxLayout(alarmWidget);
      timeLayout2->addLayout(timeLayout);
      timeLayout2->addLayout(timeLayout1);

      //点击开启闹钟，按用户输入开始计时
      QObject::connect(&onalarm, &QPushButton::clicked, [=]() {

          QString strNumber = mytime.text();
          QVariant var(strNumber);
          bool ok;
          int intValue = var.toInt(&ok);
          if (ok) {
             timer1.start(intValue*60*1000);
             mytime.setText("闹钟开启"+strNumber+"分钟");
          }
           else
          {
              mytime.setText("请输入整数");
          }

          });

      //点击关闭闹钟，计时关闭
      QObject::connect(&offalarm, &QPushButton::clicked, [=]() {

               timer1.stop();
               mytime.setText("闹钟关闭");

          });

      //计时器结束后
      QObject::connect(&timer1, &QTimer::timeout, [=]() {

          onOff->setIcon(QIcon(":/new/prefix2/off1.png"));
          onOff->setFlat(false);
          allmusicinformation->MusicPause(true);

          timer1.stop();
          mytime.setText("闹钟关闭");

          });

      //点击闹钟，显示闹钟设置页面
      QObject::connect(&alarm, &QPushButton::clicked, [=]() {
          topWidget->setCurrentIndex(8);
          });



/*********************************优化布局********************************/

      //首页歌曲推荐
      QWidget *main=new QWidget();
      main->setFixedSize(1050,700);

      //图片及其布局
      QLabel *singer1=new QLabel(main);
      QLabel *singer2=new QLabel(main);
      QLabel *singer3=new QLabel(main);
      QLabel *singer4=new QLabel(main);
      QLabel *singer5=new QLabel(main);
      QLabel *singer6=new QLabel(main);

      QPixmap pixmap11(":/singer1.jpg");
      singer1->setPixmap(pixmap11);
      singer1->setScaledContents(true); // 让图片自适应 Label 大小

      QPixmap pixmap12(":/singer2.jpg");
      singer2->setPixmap(pixmap12);
      singer2->setScaledContents(true); // 让图片自适应 Label 大小

      QPixmap pixmap13(":/singer3.jpg");
      singer3->setPixmap(pixmap13);
      singer3->setScaledContents(true); // 让图片自适应 Label 大小

      QPixmap pixmap14(":/singer4.jpg");
      singer4->setPixmap(pixmap14);
      singer4->setScaledContents(true); // 让图片自适应 Label 大小

      QPixmap pixmap15(":/singer5.jpg");
      singer5->setPixmap(pixmap15);
      singer5->setScaledContents(true); // 让图片自适应 Label 大小

      QPixmap pixmap16(":/singer6.jpg");
      singer6->setPixmap(pixmap16);
      singer6->setScaledContents(true); // 让图片自适应 Label 大小


      //歌手照片布局
      QHBoxLayout *singerlayout1=new QHBoxLayout;
      singerlayout1->addWidget(singer1);
      singerlayout1->addWidget(singer2);
      singerlayout1->addWidget(singer5);
      singerlayout1->setSpacing(15);

      QHBoxLayout *singerlayout2=new QHBoxLayout;
      singerlayout2->addWidget(singer3);
      singerlayout2->addWidget(singer4);
      singerlayout2->addWidget(singer6);
      singerlayout2->setSpacing(15);

      QVBoxLayout *singerlayout=new QVBoxLayout(main);
      singerlayout->addLayout(singerlayout1);
      singerlayout->addLayout(singerlayout2);


      first=new QPushButton("歌曲推荐");
      first->resize(40,40);
      first->setFont(songsFont);

      //显示首页的歌手推荐
      QObject::connect(first, &QPushButton::clicked, [=]() {
             topWidget->setCurrentIndex(7);
          });


/************总布局*************/

      betterLayout=new QVBoxLayout(this);
      splitterMain =new QSplitter(Qt::Horizontal,this);
      splitterMain->setOpaqueResize(true);
      betterLayout->addWidget(splitterMain);

      splitterMain->setStretchFactor(0, 1); // 左侧部分所占比例
      splitterMain->setStretchFactor(1, 5); // 右侧部分所占比例


      //总分割窗口左边
      QWidget* leftWidget=new QWidget;
      splitterMain->addWidget(leftWidget);
      QVBoxLayout* leftLayout=new QVBoxLayout(leftWidget);
      leftLayout->addWidget(returnKey);
      leftLayout->addWidget(first);
      leftLayout->addWidget(searchbtn);
      leftLayout->addWidget(lyric);
      leftLayout->addWidget(rollingLyric);
      leftLayout->addWidget(singer);
      leftLayout->addWidget(songs);
      leftLayout->addWidget(allMusic);
      leftLayout->addWidget(&alarm);
      leftLayout->addWidget(help);

      leftLayout->addStretch();


      //总分割窗口右边
      QSplitter *splitterRight =new QSplitter(Qt::Vertical,splitterMain);
      QWidget* bottomWidget=new QWidget;
      topWidget=new QStackedWidget;
      splitterRight->addWidget(topWidget);
      splitterRight->addWidget(bottomWidget);

      QHBoxLayout* bottomLayout=new QHBoxLayout(bottomWidget);
      bottomLayout->addWidget(songName);
      bottomLayout->addWidget(playingMode);
      bottomLayout->addWidget(last);
      bottomLayout->addWidget(onOff);
      bottomLayout->addWidget(next);
      bottomLayout->addWidget(musicSlider);
      bottomLayout->addWidget(musicTime);
      bottomLayout->addWidget(sound);
      bottomLayout->addWidget(soundSlider);
      bottomLayout->addWidget(noSound);
      bottomLayout->addWidget(random);


      //栈窗
      topWidget->addWidget(lyricswidget);
      topWidget->addWidget(singerWidget);
      topWidget->addWidget(albumWidget);
      topWidget->addWidget(lyricwidget);
      topWidget->addWidget(allmusicinformation->playList);
      topWidget->addWidget(chat->chatWidget);
      topWidget->addWidget(searchWidget);
      topWidget->addWidget(main);
      topWidget->addWidget(alarmWidget);
      topWidget->setCurrentIndex(7);


}




PlayMusic::~PlayMusic()
{

}


void PlayMusic::mousePressEvent(QMouseEvent *event)
{

    QPoint y =event->globalPos();  //事件发生时鼠标的全局坐标
    QPoint x=this->geometry().topLeft();   //窗口坐标
    this->z=y-x;        //鼠标按下时窗口相对于鼠标位置的初始偏移量
}


void PlayMusic::mouseReleaseEvent(QMouseEvent *event)
{
    this->z=QPoint();   //空，不再需要记录窗口与鼠标的偏移量
}


void PlayMusic::mouseMoveEvent(QMouseEvent *event)
{
     QPoint y =event->globalPos();    //事件发生时鼠标的全局坐标
     QPoint x=y-this->z;              //减去偏移量，即窗口现在坐标
     this->move(x);
}



void PlayMusic::onPositionChanged(qint64 a)
{
    QString musicPath=allmusicinformation->getMusicPath();

/**********多线程************/

    //使用QtConcurrent在单独的线程中执行文件读取和处理操作
    //使用QtConcurrent::run函数可以提高应用程序的响应性，同时主线程可以继续执行其他任务。

   auto processLRC = [=]() {
      QFile lrc(allmusicinformation->getMusicLyrics(musicPath));
      QString all;

      if (lrc.open(QIODevice::ReadOnly))
      {
          QTextStream in(&lrc);
          in.setCodec("GBK");
          all=in.readAll();
      }

      return all;
   };

   //QFuture<QString> 是 Qt 提供的用于处理异步操作和多线程编程的工具之一。
   //它用于表示一个尚未完成的操作，并提供了一种机制来获取异步操作的结果。
   QFuture<QString> future = QtConcurrent::run(processLRC);
   // 等待后台任务完成并获取结果
   QString all = future.result();

/**********label更新**********/

    if(all!="")
    {
        //歌词
        QStringList s1=all.split("\n");  //存储每一句
        for(int i=1;i<s1.size();i++)
        {
            QString ss1=s1.at(i);
            //歌词开头有些无意义字符，只保存有时间截的字符串
            QRegExp inRegExp=QRegExp("\\[\\d{2}:\\d{2}.\\d{2}\\]");  //正则表达式
            //若包含则返回false，因为返回的索引从0开始
            bool match=inRegExp.indexIn(ss1);

             if(match==false)
            {
                int minute=ss1.mid(1,2).toInt();  //分
                int second=ss1.mid(4,2).toInt();   //秒
                int centisecond=ss1.mid(7,2).toInt();  //厘秒
                int lrctime=(minute*60+second)*100+centisecond;  //总厘秒
                QString lrcstr=ss1.mid(10);
                //QMap保存
                lrcMap->insert(lrctime,lrcstr);

            }
        }
    }

    int position=static_cast<int>(allmusicinformation->player->position());
     //都以厘秒算
    int pos=position/10;
    iter=lrcMap->begin();

    while(pos>iter.key())
    {
        if(pos>iter.key()&&pos<(++iter).key())
        {
            iter--;
            break;
        }
        else
        iter++;

    }

    if(iter!=lrcMap->end())    //label[4]显示当前歌词
    {

        if(pos-50<=iter.key()&&pos+50>=iter.key())
        {

            int j=0;
            if(iter!=lrcMap->begin())
            {
                iter--;
                label3->setText(iter.value());

                j++;
            }
            if(iter!=lrcMap->begin())
            {
                iter--;
                label2->setText(iter.value());
                j++;
            }
            if(iter!=lrcMap->begin())
            {
                iter--;
                label1->setText(iter.value());
                j++;
            }
            if(iter!=lrcMap->begin())
            {
                iter--;
                label0->setText(iter.value());
                j++;
            }
            for (;j>0;j--)
            {
                iter++;
            }

            //中间
            label4->setText(iter.value());
            iter++;
            if(iter!=lrcMap->end())
            {
                label5->setText(iter.value());
                iter++;
            }
            else
            {
                label5->setText("");
               // return;
            }

            if(iter!=lrcMap->end())
            {
                label6->setText(iter.value());
                iter++;
            }
            else
            {
                label6->setText("");
               // return;
            }

            if(iter!=lrcMap->end())
            {
                label7->setText(iter.value());
                iter++;
            }
            else
            {
                label7->setText("");
               // return;
            }

            if(iter!=lrcMap->end())
            {
                label8->setText(iter.value());
                iter++;
            }
            else
            {
                label8->setText("");
               // return;
            }

            if(iter!=lrcMap->end())
            {
                label9->setText(iter.value());
                iter++;
            }
            else
            {
                label9->setText("");
                //return;
            }


        }
    }

    lrcMap->clear();

}


void PlayMusic::getLyrics()
{
        lyricwidget->clear();
        QString musicPath=allmusicinformation->getMusicPath();

/**********多线程************/

        //使用QtConcurrent在单独的线程中执行文件读取和处理操作
        //使用QtConcurrent::run函数可以提高应用程序的响应性，同时主线程可以继续执行其他任务。
       auto processLRC = [=]() {
          QFile lrc(allmusicinformation->getMusicLyrics(musicPath));
          QString all;
          if (lrc.open(QIODevice::ReadOnly))
          {
              QTextStream in(&lrc);
              in.setCodec("GBK");
              all=in.readAll();
          }
          return all;
       };

          QFuture<QString> future = QtConcurrent::run(processLRC);
          // 等待后台任务完成并获取结果
          QString all = future.result();

 /*********歌词显示***********/

        QStringList all1;
        if(all!="")
        {
            //歌词
            QStringList s1=all.split("\n");  //存储每一句
            for(int i=1;i<s1.size();i++)
            {
                QString ss1=s1.at(i);
                //歌词开头有些无意义字符，只保存有时间截的字符串
                QRegExp inRegExp=QRegExp("\\[\\d{2}:\\d{2}.\\d{2}\\]");
                //若包含则返回false
                bool match=inRegExp.indexIn(ss1);

                 if(match==false)
                {
                    all1.append(ss1.mid(10));
                }
            }

        }

      lyricwidget->addItem("\n\n");
      foreach(QString line,all1)
      {
          QListWidgetItem* item= new QListWidgetItem(line);
          //item->flags()返回了一个表示item对象属性的标志位集合，可以进行各种判断和操作。
          //通过&~Qt::ItemIsSelectable，将Qt::ItemIsSelectable对应的标志位置为0，即取消了该项的选择功能。
          //这行代码的作用是禁止item对象被选择
          item->setFlags(item->flags()&~Qt::ItemIsSelectable);

          lyricwidget->addItem(item);

      }

}


void PlayMusic::showPlayButton()
{
    if(!onOff->isFlat())
    {
        onOff->setIcon(QIcon(":/new/prefix2/on1.png"));
        onOff->setFlat(true);

        allmusicinformation->MusicPause(false);
    }
    else
    {
         onOff->setIcon(QIcon(":/new/prefix2/off1.png"));
         onOff->setFlat(false);

         allmusicinformation->MusicPause(true);
    }
}


void PlayMusic::showPlayMode()
{

    if(!playingMode->isFlat())
    {
        playingMode->setIcon(QIcon(":/new/prefix2/sequential1.png")); //循序播放
        playingMode->setFlat(true);

        allmusicinformation->MusicPause(true);
        allmusicinformation->MusicMode(true);

        if(onOff->isFlat())
        {
            allmusicinformation->MusicPause(false);
        }
    }
    else
    {
        playingMode->setIcon(QIcon(":/new/prefix2/shuffle1.png"));  //随机
        playingMode->setFlat(false);

        allmusicinformation->MusicPause(true);
        allmusicinformation->MusicMode(false);

        if(onOff->isFlat())
        {
            allmusicinformation->MusicPause(false);
        }

    }

}


void PlayMusic::showSoundSlider()
{
    if(!sound->isFlat())
    {
        soundSlider->show();
        noSound->show();
        sound->setFlat(true);

    }
    else
    {
        soundSlider->close();
        noSound->close();
        sound->setFlat(false);
    }
}


void PlayMusic::showNoSound()
{
    if(!noSound->isFlat())
    {
        noSound->setIcon(QIcon(":/new/prefix2/noSound.jpg"));
        noSound->setFlat(true);
        allmusicinformation->MusicNoSound(true);
        soundSlider->setValue(0);

    }
    else
    {
        noSound->setIcon(QIcon(":/new/prefix2/yesSound.jpg"));
        noSound->setFlat(false);
        allmusicinformation->MusicNoSound(false);
        soundSlider->setValue(50);
    }

    if(!onOff->isFlat())
    {
        allmusicinformation->MusicPause(true);

        onOff->setIcon(QIcon(":/new/prefix2/off1.png"));
    }

}


void PlayMusic::nextMusic()
{

    allmusicinformation->NextMusic();
}


void PlayMusic::laseMusic()
{

    allmusicinformation->LastMusic();
}


void PlayMusic::setSound()
{
    allmusicinformation->SetSound(soundSlider->value());
}


void PlayMusic::showLyricWidget()
{   
    //lyricwidget->show();
    //this->hide();
    topWidget->setCurrentIndex(3);
    lyric->setFlat(false);
}


void PlayMusic::showLyricsWidget()
{
    topWidget->setCurrentIndex(0);
    rollingLyric->setFlat(false);

}


void PlayMusic::showMusicTime()
{
    int time=static_cast<int>(allmusicinformation->getPlayerPosition())/1000;
    int alltime=static_cast<int>(allmusicinformation->getPlayerDuration())/1000;

    QString minute1=QString::number(time/600,10);
    QString minute2=QString::number(time/60%10,10);
    QString second1=QString::number(time%60/10,10);
    QString second2=QString::number(time%60%10,10);
    QString total=minute1+minute2+":"+second1+second2;

    QString minute11=QString::number(alltime/600,10);
    QString minute22=QString::number(alltime/60%10,10);
    QString second11=QString::number(alltime%60/10,10);
    QString second22=QString::number(alltime%60%10,10);
    QString total1=minute11+minute22+":"+second11+second22;

    musicTime->setText(total+"/"+total1);
    double per=1000*((1.0*time/alltime));
    musicSlider->setValue(per);

   // this->getLyrics();

    QString musicPath=allmusicinformation->getMusicPath();
    QString allname=allmusicinformation->getMusicName(musicPath);
    QStringList text=allname.split(" - ");

    songName->setText(text.last());

}


void PlayMusic::currentMusic()
{
    qint64 alltime=allmusicinformation->getPlayerDuration();
    allmusicinformation->CurrentMusic(alltime * musicSlider->value() / 1000.0);

}


void PlayMusic::showReturnTip()
{
    QMessageBox customMsgBox;
    customMsgBox.setWindowTitle(tr("**关闭提示**"));	//设置消息框的标题

    QPushButton *yesBtn=customMsgBox.addButton(tr("Yes"),QMessageBox::ActionRole);
    QPushButton *noBtn=customMsgBox.addButton(tr("No"),QMessageBox::ActionRole);

    customMsgBox.setText(tr("确定要关闭音乐主页面吗？"));
    customMsgBox.exec();

    if(customMsgBox.clickedButton()==yesBtn)
        this->close();
    if(customMsgBox.clickedButton()==noBtn)
        this->show();

    return;
}


void PlayMusic::showAllMusicList()
{

    //allmusicinformation->playList->show();
    topWidget->setCurrentIndex(4);
}



void PlayMusic::showHelpWindow()
{
    topWidget->setCurrentIndex(5);
}


void PlayMusic::clickListSong(const QModelIndex& index)    //点击所有音乐列表，播放音乐
{
    int row=index.row();

    allmusicinformation->musicList->setCurrentIndex(row);    //当前项的索引为选中行的索引
    allmusicinformation->player->play();

    if(!onOff->isFlat())
    {
        onOff->setIcon(QIcon(":/new/prefix2/on1.png"));
        onOff->setFlat(true);

        allmusicinformation->MusicPause(false);
    }

}


void PlayMusic::showSingerWindow()
{
    topWidget->setCurrentIndex(1);
}


void PlayMusic::showAlbumWindow()
{
    topWidget->setCurrentIndex(2);
}


void PlayMusic::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
        label0->setText("");
        label1->setText("");
        label2->setText("");
        label3->setText("");
        label4->setText("");
        label5->setText("");
        label6->setText("");
        label7->setText("");
        label8->setText("");
        label9->setText("");

        this->getLyrics();   //文本歌词同步

        //播放到最后一首就重复播放这一首
        if(allmusicinformation->musicList->currentIndex()==allmusicinformation->musicnumber-1)
        {
          allmusicinformation->player->playlist()->setCurrentIndex(allmusicinformation->musicnumber-1);
        }

}


void PlayMusic::Search()
{
    QString keyword = searchLineEdit->text();

    if(keyword=="")
    {
        return;
    }

    // 清空搜索结果表格
    resultTable->clearContents();
    resultTable->setRowCount(0);

    // 在播放列表中搜索匹配的音乐
   for (int i = 0; i <allmusicinformation->musicList->mediaCount(); ++i)
   {
       QMediaContent media =allmusicinformation->musicList->media(i);
       QString fileName = media.canonicalUrl().fileName();

       if (fileName.contains(keyword, Qt::CaseInsensitive))   //如果文件名中包含有搜索关键词
       {
          int rowCount = resultTable->rowCount();    // 添加搜索结果到表格中，表示插入的行数

          resultTable->insertRow(rowCount);
          resultTable->setItem(rowCount, 0, new QTableWidgetItem(fileName));    //第一列
          resultTable->setItem(rowCount, 1, new QTableWidgetItem(media.canonicalUrl().toString()));  //第二列
       }
   }
   result->show();

}


void PlayMusic::playSearchMusic(int row,int column)
{
    result->hide();
    searchLineEdit->clear();

    Q_UNUSED(column);
    QString filePath = resultTable->item(row, 1)->text();  //输出前面会有file:///加路径
    filePath.remove("file:///");

    int songIndex;
    //停止当前正在播放的音乐
    allmusicinformation->player->stop();

    // 遍历播放列表
    for (int i = 0; i < allmusicinformation->player->playlist()->mediaCount(); ++i)
    {
        QMediaContent media = allmusicinformation->player->playlist()->media(i);
        if (media.canonicalUrl().toLocalFile() == filePath)
        {
            //找到了匹配的歌曲，记录索引，并跳出循环
            songIndex = i;
            break;

        }

    }

    allmusicinformation->player->playlist()->setCurrentIndex(songIndex);
    allmusicinformation->player->play();

    if(!onOff->isFlat())
    {
        onOff->setIcon(QIcon(":/new/prefix2/on1.png"));
        onOff->setFlat(true);
        allmusicinformation->MusicPause(false);

    }

}


void PlayMusic::searchWindow()
{
    topWidget->setCurrentIndex(6);   //切换到搜索窗

}



