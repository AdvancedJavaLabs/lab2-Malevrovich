#include "amqp_channel.hpp"

#include <iostream>

AMQPChannel::AMQPChannel(struct ev_loop* loop, const AMQP::Address& addr)
    : loop_{loop},
      address_{addr},
      handler_{loop_},
      connection_{&handler_, addr},
      channel_{&connection_} {
    channel_.onError(
        [](std::string_view error) { std::cerr << "AMQP Error: " << error << std::endl; });
}