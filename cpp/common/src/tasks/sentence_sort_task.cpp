#include "tasks/sentence_sort_task.hpp"

#include "sentences_utils.hpp"
#include "serialization.hpp"

#include <algorithm>
#include <functional>
#include <queue>

namespace {

struct QueueElement {
    std::string sentence;
    uint32_t part_id;
    size_t index;

    QueueElement(const std::string& s, uint32_t p, size_t i)
        : sentence(s),
          part_id(p),
          index(i) {}

    bool operator>(const QueueElement& other) const {
        return sentence.size() < other.sentence.size();
    }
};

std::vector<std::string> SplitIntoSentences(const std::string& text) {
    std::vector<std::string> sentences;
    std::string current_sentence;

    for (size_t i = 0; i < text.size(); ++i) {
        current_sentence += text[i];

        if (i + 1 < text.size()) {
            if (lab2::isEndOfSentence(text[i], text[i + 1])) {
                sentences.push_back(current_sentence);
                current_sentence.clear();
            }
        } else {
            if (lab2::isSentenceEndChar(text[i])) {
                sentences.push_back(current_sentence);
                current_sentence.clear();
            }
        }
    }

    if (!current_sentence.empty()) {
        sentences.push_back(current_sentence);
    }

    return sentences;
}

std::string WriteSentences(const std::vector<std::string>& sentences) {
    std::string result;

    lab2::WriteInt(result, static_cast<int>(sentences.size()));

    for (const auto& sentence : sentences) {
        lab2::WriteString(result, sentence);
    }

    return result;
}

std::vector<std::string> ReadSentences(const std::string& data) {
    std::vector<std::string> sentences;

    if (data.empty()) {
        return sentences;
    }

    size_t offset = 0;
    int count = 0;

    if (!lab2::ReadInt(data, count, offset)) {
        return sentences;
    }

    for (int i = 0; i < count; ++i) {
        std::string sentence;
        if (!lab2::ReadString(data, sentence, offset)) {
            break;
        }
        sentences.push_back(sentence);
    }

    return sentences;
}

} // namespace

lab2::SentenceSortTask::SentenceSortTask() = default;

std::string lab2::SentenceSortTask::handleRequest(const std::string& task_arg,
                                                  const std::string& task_data) {
    std::vector<std::string> sentences = SplitIntoSentences(task_data);

    std::sort(sentences.begin(), sentences.end(),
              [](const std::string& a, const std::string& b) { return a.size() < b.size(); });

    return WriteSentences(sentences);
}

void lab2::SentenceSortTask::aggregateResult(uint32_t part_id, std::string&& data) {
    part_results_[part_id] = ReadSentences(data);
}

std::string lab2::SentenceSortTask::getAggregatedResult() {
    std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<QueueElement>> pq;

    for (auto& [part_id, sentences] : part_results_) {
        if (!sentences.empty()) {
            pq.emplace(sentences[0], part_id, 1);
        }
    }

    std::string result;

    while (!pq.empty()) {
        QueueElement current = pq.top();
        pq.pop();

        result += "|" + current.sentence + "|\n";

        if (current.index < part_results_[current.part_id].size()) {
            const std::string& next_sentence = part_results_[current.part_id][current.index];
            pq.emplace(next_sentence, current.part_id, current.index + 1);
        }
    }

    return result;
}