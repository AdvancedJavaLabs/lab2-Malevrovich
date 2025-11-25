#pragma once

#include "task.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace lab2 {

class SentenceSortTask : public TaskAggregator {
public:
    SentenceSortTask();

    static std::string handleRequest(const std::string& task_arg, const std::string& task_data);

    void aggregateResult(uint32_t part_id, std::string&& data) override;

    std::string getAggregatedResult() override;

private:
    std::unordered_map<uint32_t, std::vector<std::string>> part_results_;
};

} // namespace lab2