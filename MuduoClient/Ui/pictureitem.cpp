#include "pictureitem.h"
#include <QSize>
#define WIDTH 600
#define HEAD_WIDTH  50
#define HEAD_HEIGHT 50

PictureItem::PictureItem(QPixmap picImg, QPixmap headImg, int headId, QWidget *parent):QWidget(parent)
{
    //头像
       labelHead = new QLabel(this);
       QPixmap head(headImg);
       head = head.scaled(QSize(HEAD_WIDTH, HEAD_HEIGHT));
       labelHead->setPixmap(head);

       //图片
       labelImg = new QLabel(this);
       QPixmap picture(picImg);

      // labelImg->setScaledContents(true); // 自适应大小
       // 设置大小策略为按比例缩放
      // labelImg->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
       //labelImg->setMaximumWidth(240);
       //labelImg->setMinimumHeight(320);

      QSize picSize = picture.size();
       if (picSize.width() > 240) {
           //picSize=picSize.setWidth(320,240)    ；
           picture = picture.scaledToWidth(240);
       }
        labelImg->setPixmap(picture);
       labelImg->setMinimumHeight(320);//设置最小高度，注意自适应大小时可能小于这个宽度就换行


       switch(headId)
       {
       case host:
           hostSet();
           break;
       case guest:
           guestSet();
           break;
       }

       resize(WIDTH, labelImg->height() > HEAD_HEIGHT ? labelImg->height() : HEAD_HEIGHT);
}

void PictureItem::hostSet()
{

        labelHead->setGeometry(QRect(WIDTH-HEAD_WIDTH, 0, HEAD_WIDTH, HEAD_HEIGHT));
        labelImg->setGeometry(QRect(WIDTH-HEAD_WIDTH-labelImg->width(),0, 240, labelImg->height()));
        labelImg->move(WIDTH-HEAD_WIDTH-labelImg->width(), 0);


}

void PictureItem::guestSet()
{
    labelHead->setGeometry(QRect(0, 0, HEAD_WIDTH, HEAD_HEIGHT));
   // labelImg->setGeometry(QRect(HEAD_WIDTH,0, labelImg->width(), labelImg->height()));
    labelImg->move(HEAD_WIDTH, 0);

}

