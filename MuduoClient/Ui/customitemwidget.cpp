#include "customitemwidget.h"


CustomItemWidget::CustomItemWidget(const QIcon &icon, const QString &nickname, QWidget *parent)
    : QWidget(parent) {
        layout = new QHBoxLayout(this);
        iconLabel = new QLabel(this);
        pixmap=icon.pixmap(50,50);
        iconLabel->setPixmap(pixmap); // 设置头像图标
        layout->addWidget(iconLabel);
        m_name=nickname;
        nicknameLabel = new QLabel(m_name, this); // 设置昵称
        layout->addWidget(nicknameLabel);
 }


