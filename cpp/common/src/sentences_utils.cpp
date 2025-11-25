#include "sentences_utils.hpp"

#include <cctype>
#include <functional>
#include <string>

namespace {

void trim(std::string& str) {
    while (!str.empty() && std::isspace(static_cast<unsigned char>(str.back()))) {
        str.pop_back();
    }
}

bool isEndOfSentenceBuffer(char current_char, char next_char, bool at_buffer_end,
                           std::istream& input) {
    if (!lab2::isSentenceEndChar(current_char)) {
        return false;
    }

    char next;
    if (!at_buffer_end) {
        next = next_char;
    } else {
        next = input.peek();
    }

    return lab2::isEndOfSentence(current_char, next);
}

void addSentenceToChunk(std::string& chunk, std::string&& sentence,
                        std::function<void(const std::string&)> onChunk, size_t max_chunk_size) {
    if (chunk.empty()) {
        chunk = std::move(sentence);
    } else if (chunk.length() + 1 + sentence.length() <= max_chunk_size) {
        chunk += " " + sentence;
    } else {
        onChunk(chunk);
        chunk = std::move(sentence);
    }
}

} // namespace

bool lab2::isSentenceEndChar(char c) { return c == '.' || c == '!' || c == '?'; }

bool lab2::isEndOfSentence(char current_char, char next_char) {
    if (!isSentenceEndChar(current_char)) {
        return false;
    }

    return next_char == EOF || next_char == ' ' || next_char == '\n' || next_char == '\r' ||
           next_char == '\t' || next_char == '"' || next_char == '\'' || next_char == '\0';
}

void lab2::processTextByChunks(std::istream& input, std::function<void(const std::string&)> onChunk,
                               size_t max_chunk_size) {
    std::string chunk;
    std::string sentence;

    const size_t BUFFER_SIZE = 4096;
    sentence.reserve(BUFFER_SIZE);
    char buffer[BUFFER_SIZE];

    while (input.read(buffer, BUFFER_SIZE) || input.gcount() > 0) {
        size_t bytes_read = input.gcount();

        for (size_t i = 0; i < bytes_read; ++i) {
            char c = buffer[i];
            sentence += c;

            bool at_buffer_end = (i == bytes_read - 1);
            char next_char = at_buffer_end ? '\0' : buffer[i + 1];

            if (isEndOfSentenceBuffer(c, next_char, at_buffer_end, input)) {
                trim(sentence);

                if (!sentence.empty()) {
                    addSentenceToChunk(chunk, std::move(sentence), onChunk, max_chunk_size);
                    sentence.clear();
                }
            }
        }
    }

    if (!sentence.empty()) {
        trim(sentence);
        if (!sentence.empty()) {
            addSentenceToChunk(chunk, std::move(sentence), onChunk, max_chunk_size);
        }
    }

    if (!chunk.empty()) {
        onChunk(chunk);
    }
}

std::string lab2::normalizeWord(std::string_view word) {
    std::string normalized;

    normalized.reserve(word.length());

    std::copy_if(word.begin(), word.end(), std::back_inserter(normalized),
                 [](unsigned char c) { return std::isalnum(c) || c == '\'' || c == '-'; });

    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return normalized;
}
