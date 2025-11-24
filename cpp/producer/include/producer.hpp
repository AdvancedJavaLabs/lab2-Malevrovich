#pragma once

#include <amqpcpp/envelope.h>
#include <ev.h>
#include <istream>
#include <string>

#include "aggregator.hpp"
#include "amqp/amqp_adapter.hpp"
#include "message.hpp"
#include "sentences_utils.hpp"
#include "task.hpp"

namespace lab2 {

class Producer {
public:
    Producer(TaskType task_type, std::string task_arg)
        : task_type_{task_type},
          task_arg_{task_arg} {}

    void produce(AMQPChannelAdapter& channel_adapter, Aggregator& aggregator,
                 const std::string& request_queue, const std::string& reply_to, std::istream& input,
                 uint32_t chunk_size) {
        uint32_t produced = 0;
        processTextByChunks(
            input,
            [&, this](const std::string& chunk) {
                lab2::Message message{
                    .task_type = task_type_, .task_arg = task_arg_, .data = chunk};

                auto message_str = lab2::Message::serialize(message);
                auto envelope = AMQP::Envelope(message_str.data(), message_str.size());

                envelope.setReplyTo(reply_to);
                envelope.setCorrelationID(std::to_string(produced++));

                aggregator.notifyPublished(1);

                channel_adapter.publishToQueue(request_queue, envelope);
            },
            chunk_size);
    }

private:
    TaskType task_type_;
    std::string task_arg_;
};

} // namespace lab2