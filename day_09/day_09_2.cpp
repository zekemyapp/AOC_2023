#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

#define USE_SAMPLE 1
#define PART_TWO 0

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && (std::isdigit(*it) || *it == '-')) ++it;
	return !s.empty() && it == s.end();
}

int main (void)
{
	string filename = "input.txt";
	ifstream mFile(filename);

	if (!mFile.is_open()) {
		std::cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	std::cout << "Starting day9 execution" << endl;

	string fileContent;
	vector<string> input;

	/* Read input and create and array for ease of use */
	while (!mFile.eof()) {
		getline(mFile, fileContent);
		input.push_back(fileContent);
	}

	int64_t output = 0;
	for (string line : input) {
		vector<vector<int64_t>> sequence;

		vector<int64_t> subseq;
		stringstream ss;
		string temp;

		ss.clear();
		ss << line;
		while (!ss.eof()) {
			ss >> temp;
			if (is_number(temp)) {
				subseq.push_back(atoll(temp.c_str()));
			}
		}

		if (subseq.empty()) {
			cout << "sequence is empty" << endl;
			return -1;
		}

		while (count(subseq.begin(), subseq.end(), 0) != subseq.size()) {
			sequence.push_back(subseq);
			vector<int64_t> prev_seq = subseq;
			subseq.clear();
			for(auto it=prev_seq.begin(); it != std::prev(prev_seq.end()); it++) {
				auto next_val = std::next(it, 1);
				subseq.push_back(*next_val - *it);
			}
		}



		/* Last line of subsequences to check output */
		subseq = *std::prev(sequence.end());
		cout << "last line is: ";
		for (int64_t v : subseq) {
			cout << v << " ";
		}
		cout << endl;

		int64_t diff = *subseq.begin();
		for(auto it=sequence.rbegin(); it != std::prev(sequence.rend()); it++) {
			auto next_seq = *std::next(it, 1);
			auto current_seq = *it;
			int64_t first_val_seq = current_seq.at(0);
			int64_t first_val_next_seq = next_seq.at(0);

			int64_t new_val = first_val_next_seq - first_val_seq;
			(*std::next(it, 1)).insert((*std::next(it, 1)).begin(), new_val);
		}

		subseq = sequence.at(0);

		int64_t first_value_in_seq = subseq.at(0);
		cout << "first value in seq: " << first_value_in_seq << endl;
		output += first_value_in_seq;
	}


	std::cout << "The response part two is: " << output << endl;

	return 0;
}