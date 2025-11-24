#pragma once

#include "task.hpp"
#include <cstdint>
#include <string>

class WordCountTask : public lab2::TaskAggregator {
public:
    WordCountTask();

    static std::string handleRequest(const std::string& task_arg, const std::string& task_data);

    void aggregateResult(uint32_t part_id, std::string&& data) override;

    std::string getAggregatedResult() override;

private:
    int word_cnt_;
};