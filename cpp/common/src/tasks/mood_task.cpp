#include "tasks/mood_task.hpp"

#include <fstream>
#include <iostream>

#include <gflags/gflags.h>

#include "sentences_utils.hpp"

namespace {

void loadDictionaryFromFile(const std::string& filename,
                            std::unordered_set<std::string>& word_set) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open dictionary file: " << filename << std::endl;
        return;
    }

    std::string line;
    bool in_header = true;

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        if (line[0] == ';') {
            continue;
        }

        word_set.insert(line);
    }

    file.close();
}

void loadDictionaries(std::unordered_set<std::string>& positive_words,
                      std::unordered_set<std::string>& negative_words,
                      const std::string& directory) {
    loadDictionaryFromFile(directory + "/positive-words.txt", positive_words);
    loadDictionaryFromFile(directory + "/negative-words.txt", negative_words);
}

} // namespace

std::unordered_set<std::string> lab2::MoodTask::positive_words_;
std::unordered_set<std::string> lab2::MoodTask::negative_words_;

lab2::MoodTask::MoodTask()
    : positive_total_(0),
      negative_total_(0) {}

std::string lab2::MoodTask::handleRequest(const std::string& task_arg,
                                          const std::string& task_data) {
    static bool local_dicts_loaded = false;
    if (!local_dicts_loaded) {
        loadDictionaries(positive_words_, negative_words_,
                         "/home/malevrovich/programming/parallel/lab2-Malevrovich/cpp/resources");
        local_dicts_loaded = true;
    }

    int positive_count = 0;
    int negative_count = 0;

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
            if (positive_words_.find(normalized) != positive_words_.end()) {
                positive_count++;
            } else if (negative_words_.find(normalized) != negative_words_.end()) {
                negative_count++;
            }
        }

        start = end + 1;
    }

    return std::to_string(positive_count) + "," + std::to_string(negative_count);
}

void lab2::MoodTask::aggregateResult(uint32_t part_id, std::string&& data) {
    size_t delim_pos = data.find(',');
    if (delim_pos != std::string::npos) {
        int positive = std::stoi(data.substr(0, delim_pos));
        int negative = std::stoi(data.substr(delim_pos + 1));

        positive_total_ += positive;
        negative_total_ += negative;
    }
}

std::string lab2::MoodTask::getAggregatedResult() {
    std::string mood;
    if (positive_total_ > negative_total_) {
        mood = "POSITIVE";
    } else if (positive_total_ < negative_total_) {
        mood = "NEGATIVE";
    } else {
        mood = "NEUTRAL";
    }

    return "Positive words: " + std::to_string(positive_total_) +
           ", Negative words: " + std::to_string(negative_total_) + ", Overall mood: " + mood;
}