#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>
#include <QLabel>


class ChatItem : public QWidget
{
    Q_OBJECT

public:
    enum ChatRole
    {
        guest=0,
        host,

    };
    ChatItem(QString msg, QPixmap img,int headId, QWidget *parent = 0);
    ~ChatItem();

private:
    void hostSet();
    void guestSet();
private:
    QLabel* labelMsg;
    QLabel* labelHead;

};

#endif // CHATITEM_H
