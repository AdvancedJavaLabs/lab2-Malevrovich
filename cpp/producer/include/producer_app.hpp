#pragma once

#include <amqpcpp.h>
#include <ev.h>

#include "amqp_publisher.hpp"

class ProducerApplication {
public:
    ProducerApplication(const AMQP::Address& addr, std::string queue,
                        struct ev_loop* loop = EV_DEFAULT)
        : loop_{loop},
          publisher_{loop_, addr, [this]() { work(); }},
          queue_{std::move(queue)} {}

    void work() {
        std::cout << "Publishing..." << std::endl;
        publisher_.publish("", queue_, "Hello");
        std::cout << "Published" << std::endl;
        publisher_.stop();
        std::cout << "Stopped" << std::endl;
    }

    void run() { ev_run(loop_); }

private:
    struct ev_loop* loop_;
    AMQPPublisher publisher_;
    std::string queue_;
};