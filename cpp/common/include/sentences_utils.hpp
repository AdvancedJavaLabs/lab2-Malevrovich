#pragma once

#include <functional>
#include <iostream>
#include <string>

void processTextByChunks(std::istream& input, std::function<void(const std::string&)> onChunk,
                         size_t max_chunk_size);

bool isSentenceEndChar(char c);

bool isEndOfSentence(char current_char, char next_char);