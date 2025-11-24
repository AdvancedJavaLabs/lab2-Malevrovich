#pragma once

#include <optional>
#include <string>

#include "task.hpp"

namespace lab2 {

struct Message {
    TaskType task_type;
    std::string task_arg;
    std::string data;

    static std::string serialize(const Message& src);
    static std::optional<Message> parse(const std::string& src);
};

} // namespace lab2