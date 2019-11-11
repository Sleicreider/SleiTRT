#include "FileReader.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <utility>

void FileReader::Process(const char* path)
{
	for (auto& file : std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")))
	{
		std::ifstream content(file.path());
		std::string line;

		std::cout << file.path() << std::endl;

		while (std::getline(content, line))
		{
			std::stringstream ss;

			ss << line;

			std::string word;
			while (ss >> word)
			{
				words_[word]++;
			}
		}
	}
}

void FileReaderMutex::Process(const char* path)
{
	auto num_files = std::distance(std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")),
		std::filesystem::directory_iterator{});

	std::vector<std::thread> tt;
	tt.reserve(num_files);

	for (auto& file : std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")))
	{
		auto read = [file, this]()
		{
			std::ifstream content(file.path());
			std::string line;

			while (std::getline(content, line))
			{
				std::stringstream ss;

				ss << line;

				std::string word;
				while (ss >> word)
				{
					std::lock_guard guard(m_map_);
					words_[word]++;
				}
			}
		};

		tt.push_back(std::thread(read));
	}


	for (auto& t : tt)
	{
		t.join();
	}
}