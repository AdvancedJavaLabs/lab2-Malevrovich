#pragma once

#include "task.hpp"
#include <gflags/gflags.h>
#include <string>
#include <unordered_set>

namespace lab2 {

class MoodTask : public TaskAggregator {
public:
    MoodTask();

    static std::string handleRequest(const std::string& task_arg, const std::string& task_data);

    void aggregateResult(uint32_t part_id, std::string&& data) override;

    std::string getAggregatedResult() override;

private:
    int positive_total_;
    int negative_total_;

    static std::unordered_set<std::string> positive_words_;
    static std::unordered_set<std::string> negative_words_;
};

} // namespace lab2
