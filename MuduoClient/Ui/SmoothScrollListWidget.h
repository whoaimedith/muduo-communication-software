#ifndef SMOOTHSCROLLLISTWIDGET_H
#define SMOOTHSCROLLLISTWIDGET_H
#include <QWidget>
#include <QListWidget>
#include <QScrollBar>
#include <QPropertyAnimation>
class SmoothScrollListWidget : public QListWidget
{
    Q_OBJECT
public:
    SmoothScrollListWidget(QWidget *parent = nullptr) : QListWidget(parent)
    {
        connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &SmoothScrollListWidget::smoothScroll);
    }

protected:
    void smoothScroll(int value)
    {
        int delta = value - verticalScrollBar()->value(); // 计算滚动条值的变化量
        QPropertyAnimation *animation = new QPropertyAnimation(verticalScrollBar(), "value");
        animation->setDuration(1000); // 设置动画时长
        animation->setStartValue(verticalScrollBar()->value());
        animation->setEndValue(value + delta*3); // 将滚动条值增加
        animation->setEasingCurve(QEasingCurve::InOutQuad); // 使用平滑过渡的缓动曲线
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
};
#endif // SMOOTHSCROLLLISTWIDGET_H
