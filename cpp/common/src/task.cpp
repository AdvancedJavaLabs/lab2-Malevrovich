#include "task.hpp"

#include <memory>
#include <stdexcept>

#include "tasks/mood.hpp"
#include "tasks/word_cnt_task.hpp"

#include <optional>
#include <string_view>

std::optional<lab2::TaskType> lab2::ParseTaskType(std::string_view task_type) {
    if (task_type == "WORD_CNT") {
        return TaskType::WORD_CNT;
    } else if (task_type == "TOP_N") {
        return TaskType::TOP_N;
    } else if (task_type == "MOOD") {
        return TaskType::MOOD;
    } else if (task_type == "REPLACE") {
        return TaskType::REPLACE;
    } else if (task_type == "SORT") {
        return TaskType::SORT;
    } else {
        return std::nullopt;
    }
}

std::shared_ptr<lab2::TaskAggregator> lab2::CreateTaskAggregator(TaskType task_type) {
    switch (task_type) {
    case TaskType::WORD_CNT:
        return std::make_shared<WordCountTask>();
    case lab2::TaskType::MOOD:
        return std::make_shared<MoodTask>();
    default:
        return nullptr;
    }
}

std::string lab2::HandleTask(TaskType task_type, const std::string& task_arg,
                             const std::string& data) {
    switch (task_type) {
    case TaskType::WORD_CNT:
        return WordCountTask::handleRequest(task_arg, data);
    case lab2::TaskType::MOOD:
        return MoodTask::handleRequest(task_arg, data);
    default:
        throw std::runtime_error("Not implemented");
    }
}