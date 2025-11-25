#include "serialization.hpp"

void lab2::WriteInt(std::string& dest, int value) {
    dest += std::to_string(value);
    dest += kDelimiter;
}

bool lab2::ReadInt(const std::string& src, int& value, size_t& offset) {
    size_t value_end = src.find(kDelimiter, offset);
    if (value_end == std::string::npos) {
        return false;
    }

    std::string value_str = src.substr(offset, value_end - offset);
    try {
        value = std::stoi(value_str);
        offset = value_end + 1;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void lab2::WriteString(std::string& dest, const std::string& str) {
    WriteInt(dest, str.size()); // Используем WriteInt для записи размера
    dest += str;
    dest += kDelimiter;
}

bool lab2::ReadString(const std::string& src, std::string& str, size_t& offset) {
    int size;
    if (!ReadInt(src, size, offset)) { // Используем ReadInt для чтения размера
        return false;
    }

    if (offset + size >= src.size()) {
        return false;
    }

    str = src.substr(offset, size);
    offset += size;

    // Пропускаем завершающий разделитель
    if (offset >= src.size() || src[offset] != kDelimiter) {
        return false;
    }
    offset++;

    return true;
}