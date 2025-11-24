#pragma once

#include <memory>
#include <vector>

#include "amqp/amqp_adapter.hpp"
#include "sink.hpp"
#include "task.hpp"

namespace lab2 {

class Aggregator {
public:
    Aggregator(TaskType task_type, Sink& sink);

    void notifyPublished(uint32_t parts_count);
    void openSink();
    void aggregate(AMQPChannelAdapter& channel_adapter, std::string_view queue);

private:
    void tryFlushToSink();

    std::shared_ptr<TaskAggregator> task_aggregator_;
    Sink& sink_;
    bool sink_opened_{false};

    std::vector<bool> parts_received_{};
    uint32_t parts_left_{0};
};

} // namespace lab2