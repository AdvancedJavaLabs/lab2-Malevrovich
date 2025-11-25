#pragma once

#include <functional>
#include <iostream>
#include <string>

namespace lab2 {

void processTextByChunks(std::istream& input, std::function<void(const std::string&)> onChunk,
                         size_t max_chunk_size);

bool isSentenceEndChar(char c);

bool isEndOfSentence(char current_char, char next_char);

std::string normalizeWord(std::string_view word);

} // namespace lab2
