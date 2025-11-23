#pragma once

#include <amqpcpp/flags.h>

#include "amqp_channel.hpp"

class AMQPPublisher : private AMQPChannel {
public:
    AMQPPublisher(struct ev_loop* loop, const AMQP::Address& addr)
        : AMQPChannel(loop, addr) {}

    template <typename Action>
    AMQPPublisher(struct ev_loop* loop, const AMQP::Address& addr, Action&& onStarted)
        : AMQPChannel(loop, addr) {
        channel().onReady(std::forward<Action>(onStarted));
    }

    void publish(const std::string& exchange, const std::string& routingKey,
                 const std::string& message) {
        channel().publish(exchange, routingKey, message);
    }

    void stop() { AMQPChannel::close(); }
};