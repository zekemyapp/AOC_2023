#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>

std::unordered_map<std::string, std::vector<std::string>> memo;
std::unordered_map<std::string, int> memo_sup;

#define N_SPINS 1000000000

bool move_rock_north (std::vector<std::string> &field)
{
	bool modified = false;
	size_t rows = field.size();
	size_t cols = field.at(0).size();
	for (int i=1; i<cols; i++) {
		for (int j=0; j<cols; j++) {
			if (field.at(i).at(j) == 'O') {
				if (field.at(i-1).at(j) == '.') {
					field.at(i-1).at(j) = 'O';
					field.at(i).at(j) = '.';
					modified = true;
				}
			}
		}
	}

	if (modified) {
		move_rock_north (field);
		return true;
	}

	return false;
}

bool move_rock_south (std::vector<std::string> &field)
{
	bool modified = false;
	size_t rows = field.size();
	size_t cols = field.at(0).size();
	for (int i=cols-2; i>=0; i--) {
		for (int j=0; j<cols; j++) {
			if (field.at(i).at(j) == 'O') {
				if (field.at(i+1).at(j) == '.') {
					field.at(i+1).at(j) = 'O';
					field.at(i).at(j) = '.';
					modified = true;
				}
			}
		}
	}

	if (modified) {
		move_rock_south (field);
		return true;
	}
	return false;
}

bool move_rock_west (std::vector<std::string> &field)
{
	bool modified = false;
	size_t rows = field.size();
	size_t cols = field.at(0).size();
	for (int i=0; i<rows; i++) {
		for (int j=1; j<cols; j++) {
			if (field.at(i).at(j) == 'O') {
				if (field.at(i).at(j-1) == '.') {
					field.at(i).at(j-1) = 'O';
					field.at(i).at(j) = '.';
					modified = true;
				}
			}
		}
	}

	if (modified) {
		move_rock_west (field);
		return true;
	}
	return false;
}

bool move_rock_east (std::vector<std::string> &field)
{
	bool modified = false;
	size_t rows = field.size();
	size_t cols = field.at(0).size();
	for (int i=0; i<rows; i++) {
		for (int j=cols-2; j>=0; j--) {
			if (field.at(i).at(j) == 'O') {
				if (field.at(i).at(j+1) == '.') {
					field.at(i).at(j+1) = 'O';
					field.at(i).at(j) = '.';
					modified = true;
				}
			}
		}
	}

	if (modified) {
		move_rock_east (field);
		return true;
	}
	return false;
}

std::string merge_rock(std::vector<std::string> field)
{
	std::string out;
	for (auto l : field) {
		out += l;
	}
	return out;
}

int spin_rock(std::vector<std::string> &field, uint64_t count)
{
	std::string key = merge_rock(field);
	if (memo.find(key) != memo.end()) {
		std::vector<std::string> new_field = memo[key];
		int last_time = memo_sup[key];
		field.clear();
		copy(new_field.begin(), new_field.end(), back_inserter(field));  
		return last_time;
	}

	move_rock_north(field);
	move_rock_west(field);
	move_rock_south(field);
	move_rock_east(field);

	memo[key] = field;
	memo_sup[key] = count;
	return 0;
}

uint64_t count_rock (std::vector<std::string> &field)
{
	uint64_t count = 0;
	size_t rows = field.size();
	size_t cols = field.at(0).size();
	for (int i=0; i<cols; i++) {
		for (int j=0; j<cols; j++) {
			if (field.at(i).at(j) == 'O') {
				count += cols - i;
			}
		}
	}

	return count;
}

// 100084 TOO_HIGH

int main (void) {
	std::cout << "Starting day 14" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	std::string file_content;
	uint64_t result = 0;

	std::vector<std::string> input;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		getline(input_file, file_content);
		input.push_back(file_content);
	}

	uint64_t count = 0;
	uint64_t last_seen;
	while(true) {
		last_seen = spin_rock(input, count);
		if (last_seen > 0)
			break;
		count++;
	}

	uint64_t period = count - last_seen;
	uint64_t rest = N_SPINS - (last_seen + 1);
	rest = rest % period;

	count = 0;
	while(count < rest) {
		spin_rock(input, count);
		count++;
	}

	for (std::string s : input)
		std::cout << s << std::endl;

	result = count_rock(input);
	std::cout << "the result is " << result << std::endl;
	return 0;
}