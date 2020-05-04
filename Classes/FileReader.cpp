#include "FileReader.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <utility>

#include "FrameworkTimer.h"

void FileReader::Process(const char* path)
{
	auto b = ~(0xffffffff << static_cast<unsigned int>(15));

	auto a = (1 << (15+1)) - 1;

	FrameworkTimer t;
	auto start = t.GetCurrentTimeInMilliseconds().count();

	for (auto& file : std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")))
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
				words_[word]++;
			}
		}
	}

	std::cout << "DIFF=" << t.GetCurrentTimeInMilliseconds().count() - start << std::endl;
}

void FileReaderMutex::Process(const char* path)
{
	FrameworkTimer t;
	auto start = t.GetCurrentTimeInMilliseconds().count();

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

	std::cout << "DIFF=" << t.GetCurrentTimeInMilliseconds().count() - start << std::endl;
}

void FileReaderMutex2::Process(const char* path)
{
	FrameworkTimer t;
	auto start = t.GetCurrentTimeInMilliseconds().count();

	auto num_files = std::distance(std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")),
		std::filesystem::directory_iterator{});

	auto cores = std::thread::hardware_concurrency();
	auto per_core = num_files / cores;

	std::vector<std::thread> tt;
	tt.reserve(cores);


	std::vector<std::filesystem::directory_entry> p;
	p.reserve(per_core + 1);
	auto it = std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\"));

	for (int i = 0; i < cores; i++)
	{
		for (auto k = 0; k < per_core; k++)
		{
			p.push_back(*it);
			it++;
		}


		auto task = [p, this]()
		{
			for (auto& file : p)
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
			}
		};

		p.clear();
		tt.push_back(std::thread(task));
	}

	for (auto& t : tt)
	{
		t.join();
	}

	std::cout << "DIFF=" << t.GetCurrentTimeInMilliseconds().count() - start << std::endl;
}


void FileReaderProducerConsumerMutex::Process(const char* path)
{
	FrameworkTimer t;
	auto start = t.GetCurrentTimeInMilliseconds().count();

	auto num_files = std::distance(std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")),
		std::filesystem::directory_iterator{});

	auto cores = std::thread::hardware_concurrency();
	auto per_core = num_files / cores;

	slots_.reserve(cores);

	for (auto i = 0; i < cores; i++)
	{
		slots_.push_back(std::nullopt);
		std::thread(&FileReaderProducerConsumerMutex::Consume, this, slots_.back());
	}

}


void FileReaderProducerConsumerMutex::Produce()
{
	FrameworkTimer t;
	auto start = t.GetCurrentTimeInMilliseconds().count();

	auto num_files = std::distance(std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\")),
		std::filesystem::directory_iterator{});

	auto cores = std::thread::hardware_concurrency();
	auto per_core = num_files / cores;

	std::vector<std::thread> tt;
	tt.reserve(cores);

	std::vector<std::filesystem::directory_entry> p;
	p.reserve(per_core + 1);
	auto it = std::filesystem::directory_iterator(std::string("C:\\Users\\dreisner\\Documents\\TRT\\SleiTRT\\Resources\\"));


	while (it != std::filesystem::end(it))
	{
		for (auto& slot : slots_)
		{
			if (slot != std::nullopt)
			{
				continue;
			}

			slot = { [it] { return *it; }, false };
			it++;
		}
	}

	for (auto& slot : slots_)
	{
		slot = { nullptr, true };
	}
}

void FileReaderProducerConsumerMutex::Consume(std::optional<STask>& obama)
{
	while (!obama->done)
	{
		if (obama == std::nullopt) continue;

		auto file = obama->fn();

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
	}
}