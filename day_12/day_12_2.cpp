#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iterator>

std::unordered_map<std::string, uint64_t> memo;


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
	std::stringstream clue_str;
	std::copy(clues.begin(), clues.end(), std::ostream_iterator<int>(clue_str, ","));
	std::string key = line + ";" + clue_str.str();

	// Check cache
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int n = clues.at(0);
	size_t ref = line.find_first_of("#?");

	if (line.at(ref) == '?') {
		
	}

	std::string new_line = line.substr(ref + , std::string::npos);

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

	// uint64_t count = 0;
	// if (memo.find(new_line1) != memo.end()) {
	// 	count = memo[new_line1];
	// } else {
	// 	// Cache the result
	// 	memo[new_line1] = count_posibilities(new_line1, clues, position);
        // 	count = memo[new_line1];
	// }

	// if (memo.find(new_line2) != memo.end()) {
	// 	count += memo[new_line2];
	// } else {
	// 	// Cache the result
	// 	memo[new_line2] = count_posibilities(new_line2, clues, position+1);
	// 	count += memo[new_line2];
	// }

	uint64_t count = count_posibilities(new_line1, clues, position+1);
	count += count_posibilities(new_line2, clues, position+1);
	return count;
}

int main (void) {
	std::cout << "Starting day 11" << std::endl;

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
		std::string pre_line = file_content.substr(0, div);
		std::string pre_clus_str = file_content.substr(div+1, std::string::npos);

		std::string line = pre_line;
		std::string clus_str = pre_clus_str;

		int possibilities = 0;
		for (int i=0; i<5; i++) {
			std::cout << "testing line: " << line << "; with clues: " << clus_str << std::endl;

			std::stringstream ss(clus_str);
			std::vector<int> clues;

			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				clues.push_back(atoi(substr.c_str()));
			}

			possibilities = count_posibilities(line, clues);
			std::cout << "possibilities: " << possibilities << std::endl;

			line = line + '?' + pre_line;
			clus_str = clus_str + ',' + pre_clus_str;
		}

		result += possibilities;
	}

	std::cout << "the result is " << result << std::endl;

	return 0;
}