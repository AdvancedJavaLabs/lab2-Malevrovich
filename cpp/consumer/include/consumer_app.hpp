#pragma once

#include <format>
#include <iostream>

#include <amqpcpp.h>
#include <ev.h>

#include "amqp_receiver.hpp"

class ConsumerApplication {
public:
    ConsumerApplication(const AMQP::Address& addr, std::string_view queue,
                        struct ev_loop* loop = EV_DEFAULT)
        : loop_{loop},
          receiver_{loop_, addr} {
        receiver_.receive(queue,
                          [](const AMQP::Message& message, uint64_t deliveryTag, bool redelivered) {
                              std::cout << std::format("Received: [tag: \"{}\", body: \"{}\"]",
                                                       deliveryTag, message.body())
                                        << std::endl;
                          });
    }

    void run() { ev_run(loop_); }

private:
    struct ev_loop* loop_;
    AMQPReceiver receiver_;
};