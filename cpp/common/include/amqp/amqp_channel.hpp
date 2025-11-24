#pragma once

#include <amqpcpp.h>
#include <amqpcpp/channel.h>
#include <amqpcpp/connectionhandler.h>
#include <amqpcpp/libev.h>

namespace lab2 {

class AMQPChannel {
public:
    AMQPChannel(struct ev_loop* loop, const AMQP::Address& addr);

    const AMQP::Channel& channel() const { return channel_; }
    AMQP::Channel& channel() { return channel_; }

    void close() { connection_.close(); }

private:
    struct ev_loop* loop_;
    AMQP::Address address_;
    AMQP::LibEvHandler handler_;
    AMQP::TcpConnection connection_;
    AMQP::TcpChannel channel_;
};

} // namespace lab2