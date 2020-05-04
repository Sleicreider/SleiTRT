#pragma once

#include <unordered_map>
#include <string>
#include <mutex>
#include <optional>
#include <filesystem>

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

class FileReaderMutex2
{
public:
	void Process(const char* path);
	const auto& GetWordMap() const { return words_; }

private:
	std::unordered_map<std::string, int> words_;
	std::mutex m_map_;
};

class FileReaderProducerConsumerMutex
{
	struct STask
	{
		std::function<std::filesystem::directory_entry()> fn;
		bool done;
	};

public:
	void Process(const char* path);
	void Produce();
	void Consume(std::optional<STask>& obama);
	const auto& GetWordMap() const { return words_; }

private:





	std::unordered_map<std::string, int> words_;
	std::mutex m_map_;
	std::vector<std::optional<STask>> slots_;
};


