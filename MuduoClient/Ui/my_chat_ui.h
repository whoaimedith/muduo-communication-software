#ifndef MY_CHAT_UI_H
#define MY_CHAT_UI_H

#include <QWidget>
#include <qnchatmessage.h>
#include <QListWidgetItem>

namespace Ui {
class my_chat_ui;
}

class my_chat_ui : public QWidget
{
    Q_OBJECT

public:
    explicit my_chat_ui(QWidget *parent = nullptr);
    ~my_chat_ui();

    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
        void dealMessageTime(QString curMsgTime);
    protected:
        void resizeEvent(QResizeEvent *event);
    private slots:
        void on_pushButton_clicked();
private:
    Ui::my_chat_ui *ui;
};

#endif // MY_CHAT_UI_H
