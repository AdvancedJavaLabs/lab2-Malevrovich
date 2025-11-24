#pragma once

#include "task.hpp"
#include <cstdint>
#include <string>

class MoodTask : public lab2::TaskAggregator {
public:
    MoodTask() {}

    static std::string handleRequest(const std::string& task_arg, const std::string& task_data) {
        return task_arg;
    }

    void aggregateResult(uint32_t part_id, std::string&& data) override {}

    std::string getAggregatedResult() override { return ""; }
};