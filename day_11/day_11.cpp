#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>


int main (void) {
	std::cout << "Starting day 10" << std::endl;

	std::string filename = "sample.txt";
	std::ifstream input_file(filename);
	std::string file_content;
	std::vector<std::vector<char>> maze;
	uint64_t i,j;

	int expansion_size = 1;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		getline(input_file, file_content);
		std::vector<char> row(file_content.begin(), file_content.end());
		maze.push_back(row);
	}

	/* Isolate all points */
	int nrow = 0;
	std::vector<std::tuple<uint64_t,uint64_t>> points;
	for (auto row : maze) {
		std::vector<char>::iterator it = row.begin();
		while (it != row.end()) {
			if (*it == '#') {
				points.push_back({nrow, it - row.begin()});
			}
			it++;
		}
		nrow++;
	}

	/* Expansion */
	std::vector<bool> empty_row(maze.size(), true);
	std::vector<bool> empty_col(maze.at(0).size(), true);
	nrow = 0;
	for (auto row : maze) {
		std::vector<char>::iterator it = row.begin();
		bool is_row_empty = true;
		while (it != row.end()) {
			if (*it == '#') {
				empty_col.at(it - row.begin()) = false;
				is_row_empty = false;
			}
			it++;
		}
		if (!is_row_empty)
			empty_row.at(nrow) = false;
		nrow++;
	}

	uint64_t added_rows=0;
	uint64_t r, c;
	for(i=0; i<empty_row.size(); i++) {
		if(empty_row.at(i)) {
			for (uint64_t k=0; k<points.size(); k++) {
				std::tie(r, c) = points.at(k);
				if(r > i+added_rows) {
					points.at(k) = {r + expansion_size, c};
				}
			}
			added_rows += expansion_size;
		}
	}

	uint64_t added_cols=0;
	for(i=0; i<empty_col.size(); i++) {
		if(empty_col.at(i)) {
			for (uint64_t k=0; k<points.size(); k++) {
				std::tie(r, c) = points.at(k);
				if(c > i+added_cols) {
					points.at(k) = {r, c + expansion_size};
				}
			}
			added_cols += expansion_size;
		}
	}

	uint64_t result = 0;
	uint64_t r1, c1, r2, c2;
	for (i=0; i<points.size(); i++) {
		std::tie(r1, c1) = points.at(i);
		for(j=i+1; j<points.size(); j++) {
			std::tie(r2, c2) = points.at(j);
			int distance = abs(r2-r1) + abs (c2-c1);
			result += distance;
		}
	}


	std::cout << "the result is " << result << std::endl;

	return 0;
}