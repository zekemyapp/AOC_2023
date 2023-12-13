#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>

#define SMUDGES_ALLOWED 1

std::string get_v_str(std::vector<std::string> field, int pos)
{
	std::string out;
	for (auto l : field) {
		out += l.at(pos);
	}

	return out;
}

int count_diff_chars(std::string str1, std::string str2)
{
	size_t len = str1.size();
	int count = 0;
	for (size_t i =0; i<len; i++) {
		if (str1.at(i) != str2.at(i))
			count++;
	}
	return count;
}

bool is_h_symetric(std::vector<std::string> field, int position)
{
	size_t ncols = field.at(0).size();
	size_t n = position+1;
	size_t ltest = std::min(n, ncols-n);
	int smudges = SMUDGES_ALLOWED;

	for (size_t i=0; i<ltest; i++){
		std::string left = get_v_str(field, position-i);
		std::string right = get_v_str(field, position+1+i);

		smudges -= count_diff_chars(left, right);
	}

	if (smudges == 0)
		return true;
	else
		return false;
}

bool is_v_symetric(std::vector<std::string> field, int position)
{
	size_t rows = field.size();
	size_t n = position+1;
	size_t ltest = std::min(n, rows-n);
	int smudges = SMUDGES_ALLOWED;

	for (size_t i=0; i<ltest; i++){
		std::string up = field.at(position-i);
		std::string down = field.at(position+1+i);

		smudges -= count_diff_chars(up, down);
	}

	if (smudges == 0)
		return true;
	else
		return false;
}

int check_h_symmetry(std::vector<std::string> field)
{
	size_t ncols = field.at(0).size();
	int i;

	for (i=0; i<ncols-1; i++) {
		std::string curr = get_v_str(field, i);
		std::string next = get_v_str(field, i+1);

		if (count_diff_chars(curr, next) <= SMUDGES_ALLOWED){
			if (is_h_symetric(field, i))
				return (i+1);
		}
	}

	return 0;
}

int check_v_symmetry(std::vector<std::string> field)
{
	size_t nrows = field.size();
	int i;

	for (i=0; i<nrows-1; i++) {
		std::string curr = field.at(i);
		std::string next = field.at(i+1);

		if (count_diff_chars(curr, next) <= SMUDGES_ALLOWED){
			if (is_v_symetric(field, i))
				return (i+1);
		}
	}

	return 0;
}

int main (void) {
	std::cout << "Starting day 13" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	std::string file_content;
	uint64_t result = 0;

	std::vector<std::vector<std::string>> input;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	std::vector<std::string> temp;
	while (!input_file.eof()) {
		getline(input_file, file_content);

		if (file_content.empty()) {
			input.push_back(temp);
			temp.clear();
			continue;
		}

		temp.push_back(file_content);
	}
	input.push_back(temp);

	for (size_t i = 0; i< input.size(); i++) {
		result += check_h_symmetry(input.at(i));
		result += 100 * check_v_symmetry(input.at(i));
	}

	std::cout << "the result is " << result << std::endl;
	return 0;
}