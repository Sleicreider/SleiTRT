#include <iostream>
#include "FileReader.h"

int main()
{
	std::cout << "----------- FileReader ------------" << std::endl;
	FileReader reader;
	reader.Process("C:/Users/dreisner/Documents/TRT/SleiTRT/Resources");

	std::cout << "----------- FileReaderMutex ------------" << std::endl;
	FileReaderMutex readerm;
	readerm.Process("");

	std::cout << "----------- FileReaderMutex2 ------------" << std::endl;
	FileReaderMutex2 readerm2;
	readerm2.Process("");

	std::cout << reader.GetWordMap().size() << std::endl;
	std::cout << readerm.GetWordMap().size() << std::endl;
	std::cout << readerm2.GetWordMap().size() << std::endl;

	auto i = 0;
	std::cin >> i;
	return 0;
}
