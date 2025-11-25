#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace lab2 {

constexpr std::string_view kMagic = "LAB2";
constexpr char kDelimiter = '\n';

void WriteInt(std::string& dest, int value);

bool ReadInt(const std::string& src, int& value, size_t& offset);

void WriteString(std::string& dest, const std::string& str);

bool ReadString(const std::string& src, std::string& str, size_t& offset);

} // namespace lab2