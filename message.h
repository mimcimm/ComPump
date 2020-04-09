#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDebug>
#include <QMetaType>
#include <QByteArray>
#include <QDateTime>

class Message
{
public:
    Message();
    Message(const Message &other);
    ~Message();

    Message(const QByteArray &body);

    QByteArray body() const;
    int pumpNo() const;
    QDateTime orderDate() const;
    QDateTime currentDate() const;
    //QStringList headers() const;

private:
    QByteArray m_body;
    int m_pumpNo;
    QDateTime m_orderDate;
    QDateTime m_currentDate;
   // QStringList m_headers;
};

Q_DECLARE_METATYPE(Message);
#endif
