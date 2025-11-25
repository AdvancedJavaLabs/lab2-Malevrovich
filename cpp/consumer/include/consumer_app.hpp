#pragma once

#include <amqpcpp/envelope.h>
#include <format>

#include <amqpcpp.h>
#include <ev.h>

#include "amqp/amqp_adapter.hpp"
#include "message.hpp"
#include "task.hpp"

namespace lab2 {

class ConsumerApplication {
public:
    ConsumerApplication(const AMQP::Address& addr, std::string request_queue,
                        struct ev_loop* loop = EV_DEFAULT)
        : loop_{loop},
          amqp_{loop_, addr} {
        amqp_.receiveAsWorker(request_queue, [this](const AMQP::Message& message,
                                                    uint64_t deliveryTag, bool redelivered) {
            if (!message.hasReplyTo() || !message.hasCorrelationID()) {
                std::cerr << std::format("Got non-rpc message: \"{}\"", message.body())
                          << std::endl;
                return;
            }

            auto msg_opt = lab2::Message::parse(message.body());

            if (!msg_opt) {
                std::cerr << std::format("Got unknown message: \"{}\"", message.body())
                          << std::endl;
                return;
            }

            lab2::Message msg_result;
            msg_result.task_type = msg_opt->task_type;
            msg_result.task_arg = msg_opt->task_arg;
            msg_result.data = HandleTask(msg_opt->task_type, msg_opt->task_arg, msg_opt->data);

            auto message_str = lab2::Message::serialize(msg_result);
            AMQP::Envelope result{message_str.data(), message_str.size()};
            result.setCorrelationID(message.correlationID());

            amqp_.publishToQueue(message.replyTo(), result);
        });
    }

    void run() { ev_run(loop_); }

private:
    struct ev_loop* loop_;
    AMQPChannelAdapter amqp_;
};

} // namespace lab2