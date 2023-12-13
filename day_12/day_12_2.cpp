#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iterator>
#include <functional>
#include <numeric>

std::unordered_map<std::string, uint64_t> memo;

uint64_t count_posibilities(std::string line, std::vector<int> clues) {
	uint64_t count;

	if (clues.size() == 0 || (clues.size() == 1 and clues.at(0) == 0)) {
		if (line.find('#') == std::string::npos)
			// Only . or ? left on the string while all # have been used
			return 1;
		else
			// Used all the # too early
			return 0;
	}

	// Not enough chars to fulfill all the clues
	if (std::accumulate(clues.begin(), clues.end(), 0) > line.size())
		return 0;

	// Last character of a run
	if (line.size() == 1) {
		if (line.at(0) == '#') {
			if (clues.size() == 1 and clues.at(0) == 1)
				return 1;
			else
				return 0;
		}

		if (line.at(0) == '.')
			if (clues.size() == 0 || (clues.size() == 1 and clues.at(0) == 0))
				return 1;
			else
				return 0;

		// ???
		count = count_posibilities(".", clues);
		count += count_posibilities("#", clues);
		return count;
	}

	std::stringstream clue_str;
	std::copy(clues.begin(), clues.end(), std::ostream_iterator<int>(clue_str, ","));
	std::string key = line + ";" + clue_str.str();

	// Check cache
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	char val = line.at(0);
	std::string new_line = line.substr(1, line.size()-1);

	if (clues.at(0) == 0) {
		if (line.at(0) == '.') {
			clues.erase(clues.begin());
		} else if (line.at(0) == '#') {
			return 0;
		} else {
			count = count_posibilities("." + new_line, clues);
			count += count_posibilities("#" + new_line, clues);
			memo[key] = count;
			return count;
		}
	}

	if (val == '.') {
		count = count_posibilities(new_line, clues);
		memo[key] = count;
		return count;
	}

	if (val == '#') {
		clues.at(0) = clues.at(0) - 1;

		if (clues.at(0) > 0) {
			if (new_line.at(0) == '.')
				return 0;
			else if (new_line.at(0) == '?') {
				new_line.at(0) = '#'; // Version with '.' should be zero
				count = count_posibilities(new_line, clues);
				memo[key] = count;
				return count;
			}
		}

		count = count_posibilities(new_line, clues);
		memo[key] = count;
		return count;
	}

	// ??
	count = count_posibilities("." + new_line, clues);
	count += count_posibilities("#" + new_line, clues);
	memo[key] = count;
	return count;
}

int main (void) {
	std::cout << "Starting day 11" << std::endl;

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
		size_t div = file_content.find(' ');
		std::string pre_line = file_content.substr(0, div);
		std::string pre_clus_str = file_content.substr(div+1, std::string::npos);

		std::string line = pre_line;
		std::string clus_str = pre_clus_str;

		uint64_t possibilities = 0;
		for (int i=0; i<5; i++) {
			std::stringstream ss(clus_str);
			std::vector<int> clues;

			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				clues.push_back(atoi(substr.c_str()));
			}

			possibilities = count_posibilities(line, clues);

			line = line + '?' + pre_line;
			clus_str = clus_str + ',' + pre_clus_str;
		}

		result += possibilities;
	}

	std::cout << "the result for input is " << result << std::endl;
	return 0;
}