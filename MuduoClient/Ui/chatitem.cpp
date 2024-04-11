#include "chatitem.h"

#define WIDTH 600
#define HEAD_WIDTH  50
#define HEAD_HEIGHT 50

ChatItem::ChatItem(QString msg, QPixmap img,int headId,  QWidget *parent):
    QWidget(parent)
{
    //头像
    labelHead = new QLabel(this);
    QPixmap head(img);



    head = head.scaled(QSize(HEAD_WIDTH, HEAD_HEIGHT));
    labelHead->setPixmap(head);

    //文本
    labelMsg = new QLabel(this);
    labelMsg->setMaximumWidth(240);//设置最大宽度，注意自适应大小时可能小于这个宽度就换行
    labelMsg->setWordWrap(true);//允许自动换行
    labelMsg->setText(msg);
    labelMsg->adjustSize();//文本根据内容自适应，这里注意设置的顺序
    labelMsg->setAlignment(Qt::AlignLeft|Qt::AlignTop);//左上对齐
    labelMsg->resize(labelMsg->width(), labelMsg->height()*1.2f);//把文本的高度调大些，因为控件加入QListyWidget中后文本会被缩短
    labelMsg->setTextInteractionFlags(Qt::TextSelectableByMouse);//设置鼠标可选中文本

    switch(headId)
    {
    case host:
        hostSet();
        break;
    case guest:
        guestSet();
        break;
    }

    resize(WIDTH, labelMsg->height()>HEAD_HEIGHT?labelMsg->height():HEAD_HEIGHT);
}

ChatItem::~ChatItem()
{

}

void ChatItem::hostSet()
{
    labelHead->setGeometry(QRect(WIDTH-HEAD_WIDTH, 0, HEAD_WIDTH, HEAD_HEIGHT));
    labelMsg->move(WIDTH-HEAD_WIDTH-labelMsg->width(), 0);
}

void ChatItem::guestSet()
{
    labelHead->setGeometry(QRect(0, 0, HEAD_WIDTH, HEAD_HEIGHT));
    labelMsg->move(HEAD_WIDTH, 0);
}
