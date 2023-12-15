#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>


uint8_t hash(std::string str)
{
	uint64_t current_value = 0;
	for (auto c : str) {
		uint8_t v = c;
		current_value += v;
		current_value *= 17;
		current_value %= 256;
	}

	return (uint8_t) current_value;
}

int main (void) {
	std::cout << "Starting day 15" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	std::string file_content;
	uint64_t result = 0;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		getline(input_file, file_content);
	}

	std::stringstream ss(file_content);

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result += hash(substr);
	}

	std::cout << "the result is " << result << std::endl;
	return 0;
}