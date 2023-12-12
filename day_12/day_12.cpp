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
	std::cout << "Checking line: " << springs << " ";
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

	for (auto c : ncount) {
		std::cout << c << " ";
	}
	std::cout << std::endl;
}

bool check_springs(std::string springs, std::vector<int> clues)
{
	std::vector<int> ncount;
	parse_springs("#...#####.##....", ncount);



	if (ncount == clues)
		return true;

	return false;
}

void count_posibilities() {
	std::string line = "????#??##.?????";
	std::vector<int> clues{1, 5, 2};

	// bool is_same = check_springs(line, clues);
}

int main (void) {
	std::cout << "Starting day 11" << std::endl;

	std::string filename = "sample.txt";
	std::ifstream input_file(filename);
	std::string file_content;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		getline(input_file, file_content);
	}

	count_posibilities();

	uint64_t result;
	std::cout << "the result is " << result << std::endl;

	return 0;
}