#pragma once

#include "task.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace lab2 {

class TopNTask : public TaskAggregator {
public:
    TopNTask(int n);

    static std::string handleRequest(const std::string& task_arg, const std::string& task_data);

    void aggregateResult(uint32_t part_id, std::string&& data) override;

    std::string getAggregatedResult() override;

private:
    int n_;
    std::unordered_map<std::string, int> word_counts_;
};

} // namespace lab2