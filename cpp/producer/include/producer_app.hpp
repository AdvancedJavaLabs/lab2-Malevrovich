#pragma once

#include <amqpcpp.h>
#include <ev.h>
#include <fstream>

#include "aggregator.hpp"
#include "amqp/amqp_adapter.hpp"
#include "producer.hpp"
#include "sink.hpp"
#include "task.hpp"
#include "uuid.hpp"

namespace lab2 {

class ProducerApplication {
public:
    ProducerApplication(const AMQP::Address& addr, std::string request_queue,
                        std::ifstream input_file, std::ofstream output_file, TaskType task_type,
                        std::string task_arg, uint32_t chunk_size,
                        struct ev_loop* loop = EV_DEFAULT)
        : loop_{loop},
          amqp_{loop_, addr, [this]() { onChannelReady(); }},
          request_queue_{std::move(request_queue)},
          input_file_{std::move(input_file)},
          producer_{task_type, std::move(task_arg)},
          sink_{amqp_, std::move(output_file)},
          aggregator_{task_type, sink_},
          chunk_size_{chunk_size} {}

    void run() { ev_run(loop_); }

private:
    void onChannelReady() {
        std::string callback_queue = uuid::generate_uuid_v4();
        aggregator_.aggregate(amqp_, callback_queue);
        producer_.produce(amqp_, aggregator_, request_queue_, callback_queue, input_file_,
                          chunk_size_);
        aggregator_.openSink();
    }

    struct ev_loop* loop_;
    AMQPChannelAdapter amqp_;

    std::string request_queue_;
    std::ifstream input_file_;

    Producer producer_;
    Sink sink_;
    Aggregator aggregator_;

    uint32_t chunk_size_;
};

} // namespace lab2