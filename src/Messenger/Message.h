#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

namespace messenger {

struct Message {
    using Counter = qint64;
    QString username;
    QString collocutor;
    bool isInput;
    quint64 timestamp;
    QString data;
    QString decryptedData;
    QString hash;
    Counter counter;
    int64_t fee;
    bool isCanDecrypted = true;
    bool isConfirmed = true;
    bool isChannel = false;
    bool isDecrypted = false;
    QString channel;

    bool operator< (const Message &second) const {
        return this->counter < second.counter;
    }
};

struct ChannelInfo {
    QString title;
    QString titleSha;
    QString admin;
    uint64_t fee;
    Message::Counter counter = -1;
    bool isWriter;
};

}

#endif // MESSAGE_H
