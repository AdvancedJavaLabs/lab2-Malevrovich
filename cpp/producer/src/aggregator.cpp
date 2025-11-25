#include "aggregator.hpp"

#include <exception>
#include <iostream>
#include <iterator>

#include <amqpcpp/message.h>

#include "message.hpp"

namespace lab2 {

namespace {

std::optional<int> extractCorrelationID(const AMQP::Message& message, size_t parts_received_size) {
    if (!message.hasCorrelationID()) {
        std::cerr << "Got non-rpc result message" << std::endl;
        return std::nullopt;
    }

    int correlation_id = 0;
    try {
        correlation_id = std::stoi(message.correlationID());
    } catch (const std::exception&) {
        std::cerr << "Got non-rpc result message" << std::endl;
        return std::nullopt;
    }

    if (correlation_id < 0 || correlation_id >= parts_received_size) {
        std::cerr << "Correlation ID is out of range" << std::endl;
        return std::nullopt;
    }

    return correlation_id;
}

} // namespace

Aggregator::Aggregator(TaskType task_type, const std::string& task_arg, Sink& sink)
    : task_aggregator_{CreateTaskAggregator(task_type, task_arg)},
      sink_{sink} {}

void Aggregator::notifyPublished(uint32_t parts_count) {
    std::fill_n(std::back_inserter(parts_received_), parts_count, false);
    parts_left_ += parts_count;
}

void Aggregator::openSink() {
    sink_opened_ = true;
    tryFlushToSink();
}

void Aggregator::aggregate(AMQPChannelAdapter& channel_adapter, std::string_view queue) {
    channel_adapter.receiveAsCallback(
        queue, [this](const AMQP::Message& message, uint64_t deliveryTag, bool redelivered) {
            auto correlationID = extractCorrelationID(message, parts_received_.size());
            if (!correlationID) {
                return;
            }

            if (!parts_received_[*correlationID]) {
                parts_received_[*correlationID] = true;
                --parts_left_;

                auto msg_opt = lab2::Message::parse(message.body());

                if (!msg_opt) {
                    std::cerr << "Wrong message format" << std::endl;
                    return;
                }

                task_aggregator_->aggregateResult(*correlationID, std::move(msg_opt->data));

                tryFlushToSink();
            }
        });
}

void Aggregator::tryFlushToSink() {
    if (sink_opened_ && parts_left_ == 0) {
        sink_.saveResult(task_aggregator_->getAggregatedResult());
    }
}

} // namespace lab2