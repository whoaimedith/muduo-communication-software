#ifndef CHATITEMDELEGATE_H
#define CHATITEMDELEGATE_H

#include <QApplication>
#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOptionViewItemV4>

class ChatItemDelegate : public QStyledItemDelegate
{
public:
    ChatItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
          //QStyledItemDelegate::paint(painter, option, index);

        QStyleOptionViewItemV4 opt = option;
            initStyleOption(&opt, index);

            painter->save();

            // 绘制气泡
QRect bubbleRect = option.rect;
// if (index.data(Qt::UserRole).toBool()) {
//     painter->setBrush(QColor(173, 216, 230)); // 浅蓝色
// } else {
//     painter->setBrush(QColor(255, 218, 185)); // 淡橙色
// }
// painter->setPen(Qt::black);
// painter->drawRoundedRect(bubbleRect, 10, 10);
//
            // 绘制消息内容
            QString text = index.data(Qt::DisplayRole).toString();
            QRect textRect = bubbleRect.adjusted(10, 10, -10, -10);
            painter->drawText(textRect, Qt::TextWordWrap, text);

            painter->restore();
    }
};
#endif // CHATITEMDELEGATE_H
