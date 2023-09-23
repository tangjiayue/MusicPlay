#include "chat.h"
#include<QVBoxLayout>
#include<QString>
#include<QBitmap>
#include<QPainter>
#include<QFont>

Chat::Chat()
{

  chatWidget=new QWidget(this);
  messageInput = new QLineEdit(this);
  chatLog = new QTextEdit(this);
  chatLog->setReadOnly(true);

  chatWidget->resize(1200,1000);

  //背景
  QFont font;
  font.setPointSize(17);
  chatWidget->setFont(font);

  //圆角
  QBitmap bmp1(chatWidget->size());
  bmp1.fill();
  QPainter p1(&bmp1);
  p1.setPen(Qt::NoPen);
  p1.setBrush(Qt::black);
  p1.drawRoundedRect(bmp1.rect(),20,20);
  chatWidget->setMask(bmp1);

  //布局
  QVBoxLayout *layout = new QVBoxLayout(chatWidget);
  layout->addWidget(chatLog,3);
  layout->addWidget(messageInput,1);

  chatLog->append("在此页面最下方可以向我提问，如果有问题，欢迎随时提问！\n");

  connect(messageInput, &QLineEdit::returnPressed, this, &Chat::sendMessage);

}

void Chat::sendMessage()
{

    //用户问题
    QString message = messageInput->text();

    chatLog->append("用户问题: " + message);

    /*************************回答****************************/

    QString reply1= "\n  对不起，原谅我听不懂你的问题 "
                    "\n  我只能回答关于音乐播放器的专辑，歌手，滚动歌词，文本歌词，声音，音乐进度，全部音乐等类似信息。"
                    "如果你得不到想要的答案，可以换一个关键词试一下哦！"
                    "\n  而且我只支持中文关键字，欢迎随时问我！";

    QString reply2="\n  专辑按钮在左边，点击进去可以看到专辑名称，歌曲已经按专辑分类好，可以进入选择你想播放的歌曲！";
    QString reply3="\n  歌手按钮在左边，点击进去可以看到歌手名称，歌曲已经按歌手分类好，可以进入选择你想播放的歌曲！";
    QString reply4="\n  滚动歌词按钮在左边，点击可以回到滚动歌词界面，实现更好的视觉体验！"
                   "\n或者你还可能想了解整个歌词，可以点击左边的文本歌词按钮，查看全部歌词。";

    QString reply5="\n  如果你想调声音，在右下角有声音按键，点击后可通过滑条调节声音，当然点击静音键你还可以实现静音。";
    QString reply6="\n  如果你想调节音乐进度，可以拉动歌曲进度条，进度条就在下方。";
    QString reply7="\n  全部歌曲按钮在左边，可能是因为它是一个音乐图，你没有发现，点击进去可以看到全部歌曲，可以选择你想播放的歌曲！";
    QString reply8="\n  如果你想调到上一首音乐，上一曲功能可以切换到上一首歌曲，按钮在下方，祝你听歌愉快。";
    QString reply9="\n  如果你想调到下一首音乐，下一曲功能可以切换到下一首歌曲，按钮在下方，祝你听歌愉快。";
    QString reply10="\n  如果你想定个闹钟，时间到了音乐停止，可以去左边点击闹钟，设置你的闹钟，记得输入时间是整数，单位是分钟哦。";
    QString reply11="\n  如果你想搜索音乐，可以点击左边的搜索，输入关键词——越详细寻找的越快哦，点击搜索或者点击回车键都可以获得搜索到的信息哦。";
    QString reply12="\n  左边推荐歌曲里面有一些好听的音乐，可以参考一下，享受音乐。";


    if(message.contains("专辑"))
    {
       chatLog->append("解答: " + reply2 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("歌手"))
    {
       chatLog->append("解答: " + reply3 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("歌词"))
    {
       chatLog->append("解答: " + reply4 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("声音"))
    {
       chatLog->append("解答: " + reply5 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("音乐进度"))
    {
       chatLog->append("解答: " + reply6 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("全部歌曲")||(message.contains("播放列表")))
    {
       chatLog->append("解答: " + reply7 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("上一首"))
    {
       chatLog->append("解答: " + reply8 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("下一首"))
    {
       chatLog->append("解答: " + reply9 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("闹钟"))
    {
       chatLog->append("解答: " + reply10 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("搜索"))
    {
       chatLog->append("解答: " + reply11 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else if(message.contains("推荐"))
    {
       chatLog->append("解答: " + reply12 +"\n"+"  希望我的解答对你有帮助，如果有问题，欢迎继续提问！\n");
    }
    else
    {
       chatLog->append("解答: " + reply1 +"\n");
    }


    //发送后就清除信息
    messageInput->clear();
}
