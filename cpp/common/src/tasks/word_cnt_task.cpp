#include "tasks/word_cnt_task.hpp"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

WordCountTask::WordCountTask()
    : word_cnt_{0} {}

std::string WordCountTask::handleRequest(const std::string& task_arg,
                                         const std::string& task_data) {
    std::stringstream stream(task_data);
    auto res = std::to_string(std::distance(std::istream_iterator<std::string>(stream),
                                            std::istream_iterator<std::string>()));
    // std::cout << "Result: " << res << std::endl;
    return res;
}

void WordCountTask::aggregateResult(uint32_t part_id, std::string&& data) {
    // std::cout << "Result: " << data << std::endl;
    word_cnt_ += std::stoi(data);
}

std::string WordCountTask::getAggregatedResult() { return std::to_string(word_cnt_); }
