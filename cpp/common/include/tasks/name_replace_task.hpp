#pragma once

#include "task.hpp"
#include <map>
#include <string>

namespace lab2 {

class NameReplaceTask : public TaskAggregator {
public:
    NameReplaceTask();

    static std::string handleRequest(const std::string& task_arg, const std::string& task_data);

    void aggregateResult(uint32_t part_id, std::string&& data) override;

    std::string getAggregatedResult() override;

private:
    std::map<uint32_t, std::string> processed_parts_;
};

} // namespace lab2
