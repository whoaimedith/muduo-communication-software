#ifndef PICTUREITEM_H
#define PICTUREITEM_H

#include <QObject>
#include <QWidget>
#include <QLabel>
class PictureItem : public QWidget
{
    Q_OBJECT
public:
    enum ChatRole
    {
        guest=0,
        host,

    };
    PictureItem(QPixmap picImg, QPixmap headImg,int headId, QWidget *parent = 0);
private:
    void hostSet();
    void guestSet();
private:
    QLabel* labelImg;
    QLabel* labelHead;
};

#endif // PICTUREITEM_H
