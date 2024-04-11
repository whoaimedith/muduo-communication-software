#ifndef MY_CHAT_WINDOW_H
#define MY_CHAT_WINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "qnchatmessage.h"

namespace Ui {
class My_Chat_Window;
}

class My_Chat_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit My_Chat_Window(QWidget *parent = nullptr);
    ~My_Chat_Window();
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
        void dealMessageTime(QString curMsgTime);
    protected:
        void resizeEvent(QResizeEvent *event);
    private slots:
        void on_pushButton_clicked();


private:
    Ui::My_Chat_Window *ui;
};

#endif // MY_CHAT_WINDOW_H
