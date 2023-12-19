#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>

#include <stdlib.h>

typedef std::tuple<int64_t, int64_t> Position;

class Line {
	public:
		Position start;
		Position end;
		std::string color;

		Line(Position s, Position e, std::string c) : start(s), end(e), color(c) {}
};

int64_t matrix_solve (Position a, Position b)
{
	int64_t x1, x2, y1, y2;
	std::tie(y1, x1) = a;
	std::tie(y2, x2) = b;

	return (x1 * y2) - (x2 * y1);
}

int main (void) {
	std::cout << "Starting day 18" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

	std::vector<std::string> field;
	std::vector<Line> lines;
	Position last_pos = Position(0, 0);
	Position pos;
	int64_t i,j;
	int64_t boundaries = 0;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);

		char dir = file_content.at(0);
		size_t div1 = file_content.find_first_of(' ');
		size_t div2 = file_content.find_first_of(' ', div1+1);
		std::string sholes = file_content.substr(div1+1, div2-(div1+1));
		int64_t nholes = atoi(sholes.c_str());
		div1 = file_content.find_first_of("#");
		std::string hdistance = file_content.substr(div1+1, 5);
		std::string hdir = file_content.substr(div1+1+5, 1);

		std::stringstream ss;
		ss << std::hex << hdistance;
		ss >> nholes;

		switch (hdir.c_str()[0])
		{
		case '0':
			dir = 'R';
			break;
		case '1':
			dir = 'D';
			break;
		case '2':
			dir = 'L';
			break;
		case '3':
			dir = 'U';
			break;
		}

		std::tie(i,j) = last_pos;
		int64_t n_i = i, n_j = j;
		switch (dir) {
			case 'U':
				n_i = i - nholes;
				break;
			case 'R':
				n_j = j + nholes;
				break;
			case 'D':
				n_i = i + nholes;
				break;
			case 'L':
				n_j = j - nholes;
				break;
		}

		boundaries += nholes;

		pos = Position(n_i, n_j);
		lines.push_back(Line(last_pos, pos, ""));
		last_pos = pos;
	}

	int64_t a2 = 0;
	for (auto l : lines) {
		a2 += matrix_solve(l.start, l.end);
	}

	int64_t area = a2 / 2;
	int64_t internal = area + 1 - (boundaries/2);
	result_one = internal + boundaries;

	std::cout << "area " << area << std::endl;
	std::cout << "boundaries " << boundaries << std::endl;
	std::cout << "internal " << internal << std::endl;

	std::cout << "the result one is " << result_one << std::endl;
	return 0;
}