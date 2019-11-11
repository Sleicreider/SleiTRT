#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

class FileReader
{
public:
	void Process(const char* path);
	const auto& GetWordMap() const { return words_; }

private:
	std::unordered_map<std::string, int> words_;
};

class FileReaderMutex
{
public:
	void Process(const char* path);
	const auto& GetWordMap() const { return words_; }

private:
	std::unordered_map<std::string, int> words_;
	std::mutex m_map_;
};