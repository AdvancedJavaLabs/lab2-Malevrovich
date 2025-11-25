#include "tasks/top_n_task.hpp"
#include "sentences_utils.hpp"
#include "serialization.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

std::string WriteMap(const std::unordered_map<std::string, int>& counts) {
    std::string result;

    lab2::WriteInt(result, static_cast<int>(counts.size()));

    for (const auto& [word, count] : counts) {
        lab2::WriteString(result, word);
        lab2::WriteInt(result, count);
    }

    return result;
}

std::unordered_map<std::string, int> ReadMap(const std::string& data) {
    std::unordered_map<std::string, int> result;

    if (data.empty()) {
        return result;
    }

    size_t offset = 0;
    int count = 0;

    if (!lab2::ReadInt(data, count, offset)) {
        return result;
    }

    for (int i = 0; i < count; ++i) {
        std::string word;
        int value = 0;

        if (!lab2::ReadString(data, word, offset)) {
            break;
        }
        if (!lab2::ReadInt(data, value, offset)) {
            break;
        }

        result[word] = value;
    }

    return result;
}

} // namespace

lab2::TopNTask::TopNTask(int n)
    : n_(n) {}

std::string lab2::TopNTask::handleRequest(const std::string& task_arg,
                                          const std::string& task_data) {
    std::unordered_map<std::string, int> local_counts;
    std::string_view data_view(task_data);
    size_t start = 0;
    size_t end = 0;

    while (end < data_view.length()) {
        while (start < data_view.length() && std::isspace(data_view[start])) {
            start++;
        }

        if (start >= data_view.length())
            break;

        end = start;
        while (end < data_view.length() && !std::isspace(data_view[end])) {
            end++;
        }

        std::string_view word_view = data_view.substr(start, end - start);
        std::string normalized = normalizeWord(word_view);

        if (!normalized.empty()) {
            local_counts[std::move(normalized)]++;
        }

        start = end + 1;
    }

    return WriteMap(local_counts);
}

void lab2::TopNTask::aggregateResult(uint32_t part_id, std::string&& data) {
    auto local_counts = ReadMap(data);

    for (const auto& [word, count] : local_counts) {
        word_counts_[word] += count;
    }
}

std::string lab2::TopNTask::getAggregatedResult() {
    std::vector<std::pair<int, std::string_view>> word_vector;
    for (const auto& [word, count] : word_counts_) {
        word_vector.emplace_back(count, word);
    }

    std::sort(word_vector.rbegin(), word_vector.rend());

    size_t result_size = std::min(static_cast<size_t>(n_), word_vector.size());

    std::stringstream result;
    for (size_t i = 0; i < result_size; ++i) {
        result << word_vector[i].second << ":" << word_vector[i].first << "\n";
    }

    return result.str();
}