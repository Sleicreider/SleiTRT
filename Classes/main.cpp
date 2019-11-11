#include <iostream>
#include "FileReader.h"

int main()
{
	std::cout << "Hello World" << std::endl;


	//FileReader reader;
	//reader.Process("C:/Users/dreisner/Documents/TRT/SleiTRT/Resources");

	FileReaderMutex readerm;
	readerm.Process("");

	std::cout << readerm.GetWordMap().size() << std::endl;

	auto i = 0;
	std::cin >> i;
	return 0;
}
