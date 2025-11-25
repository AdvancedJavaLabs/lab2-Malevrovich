#include "tasks/name_replace_task.hpp"

#include <regex>
#include <sstream>

lab2::NameReplaceTask::NameReplaceTask() = default;

std::string lab2::NameReplaceTask::handleRequest(const std::string& task_arg,
                                                 const std::string& task_data) {
    std::regex name_regex("\\b[A-Z][a-z]*\\b");

    std::string result = std::regex_replace(task_data, name_regex, task_arg);

    return result;
}

void lab2::NameReplaceTask::aggregateResult(uint32_t part_id, std::string&& data) {
    processed_parts_[part_id] = std::move(data);
}

std::string lab2::NameReplaceTask::getAggregatedResult() {
    std::stringstream final_result;
    for (const auto& [part_id, text] : processed_parts_) {
        final_result << text;
    }
    return final_result.str();
}