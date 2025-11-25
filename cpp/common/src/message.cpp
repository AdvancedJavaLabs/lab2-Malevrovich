#include "message.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "serialization.hpp"

namespace lab2 {

std::string Message::serialize(const Message& src) {
    std::string result;

    result.append(kMagic);
    result += kDelimiter;

    result += std::to_string(static_cast<uint32_t>(src.task_type));
    result += kDelimiter;

    WriteString(result, src.task_arg);

    WriteString(result, src.data);

    return result;
}

std::optional<Message> Message::parse(const std::string& src) {
    size_t offset = 0;

    if (src.size() < kMagic.size() + 1 || src.substr(0, kMagic.size()) != kMagic ||
        src[kMagic.size()] != kDelimiter) {
        std::cerr << "Wrong magic" << std::endl;
        return std::nullopt;
    }
    offset = kMagic.size() + 1;

    size_t type_end = src.find(kDelimiter, offset);
    if (type_end == std::string::npos) {
        std::cerr << "Wrong format: cannot find task_type delimiter" << std::endl;
        return std::nullopt;
    }

    std::string type_str = src.substr(offset, type_end - offset);
    try {
        uint32_t task_type = std::stoul(type_str);
        offset = type_end + 1;

        std::string task_arg;
        if (!ReadString(src, task_arg, offset)) {
            std::cerr << "Wrong format: cannot read task_arg" << std::endl;
            return std::nullopt;
        }

        std::string data;
        if (!ReadString(src, data, offset)) {
            std::cerr << "Wrong format: cannot read data" << std::endl;
            return std::nullopt;
        }

        return Message{static_cast<TaskType>(task_type), std::move(task_arg), std::move(data)};

    } catch (const std::exception& e) {
        std::cerr << "Wrong format: invalid task_type - " << e.what() << std::endl;
        return std::nullopt;
    }
}

} // namespace lab2