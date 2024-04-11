#ifndef CUSTOMITEMWIDGET_H
#define CUSTOMITEMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QIcon>

// 创建自定义的QWidget，包含头像图标和昵称
class CustomItemWidget : public QWidget {
     Q_OBJECT
public:
    CustomItemWidget()=delete;
    CustomItemWidget(const QIcon &icon, const QString &nickname, QWidget *parent = nullptr) ;

    std::string getNickName()const{return m_name.toStdString();}
    QPixmap getHeadImg()const {return pixmap;}
private:
    QHBoxLayout *layout;
    QLabel *iconLabel;
    QString m_name;//学号+姓名
    QLabel *nicknameLabel;
    QPixmap pixmap;//头像
};
#endif // CUSTOMITEMWIDGET_H
