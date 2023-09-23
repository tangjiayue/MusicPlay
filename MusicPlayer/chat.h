#ifndef CHAT_H
#define CHAT_H

#include<QDialog>
#include<QLineEdit>
#include<QTextEdit>
#include<QWidget>


class Chat :public QDialog
{
public:
    Chat();

    QLineEdit *messageInput;   //输入信息
    QTextEdit *chatLog;        //输出答案
    QWidget *chatWidget;       //帮助页面窗口



public slots:
    void sendMessage();


};

#endif // CHAT_H
