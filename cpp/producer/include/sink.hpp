#pragma once

#include "amqp/amqp_adapter.hpp"

#include <fstream>
#include <string>

namespace lab2 {

class Sink {
public:
    Sink(AMQPChannelAdapter& channel, std::ofstream&& out)
        : channel_{channel},
          out_{std::move(out)} {}

    void saveResult(const std::string& result) {
        out_ << result;
        channel_.stop();
    }

private:
    AMQPChannelAdapter& channel_;
    std::ofstream out_;
};
} // namespace lab2