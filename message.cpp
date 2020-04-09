#include "message.h"

Message::Message()
{
}

Message::Message(const Message &other)
{
    m_body = other.m_body;
}

Message::~Message()
{
}

Message::Message(const QByteArray &body)
{
    m_body = body;
    //m_headers = headers;
}

QByteArray Message::body() const
{
    return m_body;
}

int Message::pumpNo() const
{
    return m_pumpNo;
}

QDateTime Message::orderDate() const
{
    return m_orderDate;
}

QDateTime Message::currentDate() const
{
    return m_currentDate;
}
