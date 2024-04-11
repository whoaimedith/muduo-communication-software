#ifndef CHAT_MODEL_H
#define CHAT_MODEL_H
#include <QString>
#include <QAbstractListModel>
#include <QList>

// 创建一个自定义的模型来保存聊天消息
class ChatModel : public QAbstractListModel
{
public:
    struct Message {
        QString text;
        bool fromUser1; // 用于区分消息是来自用户1还是用户2
    };

    QList<Message> messages;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return messages.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= messages.count())
            return QVariant();

        const Message &message = messages.at(index.row());
        if (role == Qt::DisplayRole) {
            return message.text;
        } else if (role == Qt::UserRole) {
            return message.fromUser1;
        }

        return QVariant();
    }
};
#endif // CHAT_MODEL_H
