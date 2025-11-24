#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace lab2 {

enum class TaskType { WORD_CNT, TOP_N, MOOD, REPLACE, SORT };

std::optional<TaskType> ParseTaskType(std::string_view task_type);

std::string HandleTask(TaskType task_type, const std::string& task_arg, const std::string& data);

class TaskAggregator {
public:
    virtual void aggregateResult(uint32_t part_id, std::string&& data) = 0;
    virtual std::string getAggregatedResult() = 0;

    virtual ~TaskAggregator() = default;
};

std::shared_ptr<TaskAggregator> CreateTaskAggregator(TaskType task_type);

} // namespace lab2
