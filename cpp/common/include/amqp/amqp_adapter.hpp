#pragma once

#include <amqpcpp/envelope.h>
#include <amqpcpp/flags.h>
#include <optional>

#include "amqp_channel.hpp"

namespace lab2 {

class AMQPChannelAdapter : private AMQPChannel {
public:
    AMQPChannelAdapter(struct ev_loop* loop, const AMQP::Address& addr)
        : AMQPChannel(loop, addr) {}

    template <typename Action>
    AMQPChannelAdapter(struct ev_loop* loop, const AMQP::Address& addr, Action&& onStarted)
        : AMQPChannel(loop, addr) {
        channel().onReady(std::forward<Action>(onStarted));
    }

    void publishToQueue(const std::string& queue, const std::string& message) {
        channel().publish("", queue, message);
    }

    void publishToQueue(const std::string& queue, const AMQP::Envelope& message) {
        channel().publish("", queue, message);
    }

    template <typename Action>
    void receiveAsWorker(std::string_view queue, Action&& receiver) {
        receive(queue, std::forward<Action>(receiver), 1, AMQP::autodelete);
    }

    template <typename Action>
    void receiveAsCallback(std::string_view queue, Action&& receiver) {
        receive(queue, std::forward<Action>(receiver), std::nullopt, AMQP::exclusive);
    }

    void stop() { AMQPChannel::close(); }

private:
    template <typename Action>
    void receive(std::string_view queue, Action&& receiver, std::optional<int> prefetch_count,
                 int flags) {
        if (prefetch_count) {
            channel().setQos(*prefetch_count);
        }

        channel()
            .declareQueue(queue, flags)
            .onSuccess([this, queue, receiver = std::forward<Action>(receiver)]() {
                channel().consume(queue).onReceived(
                    [this, receiver = std::move(receiver)](const AMQP::Message& message,
                                                           uint64_t deliveryTag, bool redelivered) {
                        std::move(receiver)(message, deliveryTag, redelivered);
                        channel().ack(deliveryTag);
                    });
            });
    }
};
} // namespace lab2