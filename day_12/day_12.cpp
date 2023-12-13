#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>

/* count the number of broken springs in a complete line
   and stores it in ncount */
void parse_springs(std::string springs, std::vector<int> &ncount)
{
	// std::cout << "Checking line: " << springs << " ";
	size_t nunknown = std::count(springs.begin(), springs.end(), '?');
	if (nunknown > 0)
		return;

	size_t first = springs.find_first_of('#');

	while (first != std::string::npos) {
		size_t last = springs.find_first_of('.', first);
		if (last == std::string::npos)
			last = springs.size();
		int count = last - first;
		ncount.push_back(count);
		first = springs.find_first_of('#', last);
	}

	// for (auto c : ncount) {
	// 	std::cout << c << " ";
	// }
	// std::cout << std::endl;
}

bool check_springs(std::string springs, std::vector<int> clues)
{
	std::vector<int> ncount;
	parse_springs(springs, ncount);

	if (ncount == clues)
		return true;

	return false;
}

uint64_t count_posibilities(std::string line, std::vector<int> clues, int position = 0) {
	if (position == line.size()) {
		if (check_springs(line, clues))
			return 1;
		return 0;
	}

	char c = line.at(position);
	if (c != '?') {
		return count_posibilities(line, clues, position + 1);
	}

	std::string new_line1(line);
	new_line1.at(position) = '#';
	std::string new_line2(line);
	new_line2.at(position) = '.';

	uint64_t count = count_posibilities(new_line1, clues, position+1);
	count += count_posibilities(new_line2, clues, position+1);
	return count;
}

int main (void) {
	std::cout << "Starting day 12" << std::endl;

	std::string filename = "sample.txt";
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
		size_t div = file_content.find(' ');
		std::string line = file_content.substr(0, div);
		std::string clus_str = file_content.substr(div, std::string::npos);

		std::stringstream ss(clus_str);
		std::vector<int> clues;

		while (ss.good()) {
			std::string substr;
			getline(ss, substr, ',');
			clues.push_back(atoi(substr.c_str()));
		}

		int possibilities = count_posibilities(line, clues);
		result += possibilities;

		std::cout << "possibilities: " << possibilities << std::endl;
	}

	std::cout << "the result is " << result << std::endl;

	return 0;
}