#pragma once

#include <amqpcpp/flags.h>

#include "amqp_channel.hpp"

class AMQPReceiver : private AMQPChannel {
public:
    AMQPReceiver(struct ev_loop* loop, const AMQP::Address& addr)
        : AMQPChannel(loop, addr) {}

    template <typename Action>
    void receive(std::string_view queue, Action&& receiver) {
        channel()
            .declareQueue(queue, AMQP::autodelete)
            .onSuccess([this, queue, receiver = std::forward<Action>(receiver)]() {
                std::cout << "Receiving..." << std::endl;
                channel().consume(queue, AMQP::noack).onReceived(std::move(receiver));
            });
    }
};