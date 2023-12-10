#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>

enum dir {N, S, E, W, ERROR};

enum dir next_step(
			std::vector<std::vector<char>> maze, std::tuple<int,int> pos,
			enum dir from) {
		int i,j;
		std::tie(i, j) = pos;
		char v = maze.at(i).at(j);

		if (v == '|') {
			if (from == N)
				return S;
			else
				return N;
		}

		if (v == '-') {
			if (from == W)
				return E;
			else
				return W;
		}

		if (v == 'L') {
			if (from == N)
				return E;
			else
				return N;
		}

		if (v == 'J') {
			if (from == N)
				return W;
			else
				return N;
		}

		if (v == '7') {
			if (from == S)
				return W;
			else
				return S;
		}

		if (v == 'F') {
			if (from == S)
				return E;
			else
				return S;
		}

		return ERROR;
}

enum dir from_dir (enum dir to_dir) {
	switch (to_dir) {
		case N:
			return S;
		case S:
			return N;
		case W:
			return E;
		case E:
			return W;
		default:
			return ERROR;
	}
}


std::tuple<int,int> get_next_pos(
		std::vector<std::vector<char>> maze, std::tuple<int,int> pos,
		enum dir to_dir)
{
	int i,j;
	std::tie(i, j) = pos;

	switch (to_dir) {
		case N:
			return {i-1, j};
		case S:
			return {i+1, j};
		case W:
			return {i, j-1};
		case E:
			return {i, j+1};
		default:
			return {i, j};
	}
}

std::tuple<int,int> get_right(std::vector<std::vector<char>> maze, std::tuple<int,int> pos, enum dir from) {
	int i,j;
	std::tie(i, j) = pos;
	char v = maze.at(i).at(j);

	if (v == '|') {
			if (from == N)
				return {i, j-1};
			else
				return {i, j+1};
		}

		if (v == '-') {
			if (from == W)
				return {i+1,j};
			else
				return {i-1,j};
		}

		if (v == 'L') {
			if (from == N)
				return {i,j-1};
			else
				return {i,j+1};
		}

		if (v == 'J') {
			if (from == N)
				return {i,j-1};
			else
				return {i,j+1};
		}

		if (v == '7') {
			if (from == S)
				return {i,j+1};
			else
				return {i+1,j};
		}

		if (v == 'F') {
			if (from == S)
				return {i,j+1};
			else
				return {i-1,j};
		}

		return {i,j};
}


int main (void) {
	std::cout << "Starting day 10" << std::endl;

	std::string filename = "input.txt";
	char start_pipe = '-';				// Found manually
	enum dir start_dir = W;				// Found manually

	std::ifstream input_file(filename);
	std::string file_content;
	std::vector<std::vector<char>> maze;
	std::vector<std::tuple<int,int>> steps;
	std::vector<std::tuple<int,int>> righthand;
	int i,j;

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

	/* Find S */
	std::tuple<int,int> start_pos;
	int nrow = 0;
	for (auto row : maze) {
		std::vector<char>::iterator it = std::find(row.begin(), row.end(), 'S');
		if (it != row.end()) {
			start_pos = {nrow, it - row.begin()};
			break;
		}
		nrow++;
	}

	/* Replace start pipe */
	std::tie(i, j) = start_pos;
	maze.at(i).at(j) = start_pipe;
	std::cout << "starting pipe: " << maze.at(i).at(j) << std::endl;

	steps.push_back(start_pos);

	enum dir next_dir = next_step(maze, start_pos, start_dir);
	enum dir prev_dir = from_dir(next_dir);
	std::tuple<int,int> pos = get_next_pos(maze, start_pos, next_dir);
	std::tuple<int,int> right_pos;
	while (pos != start_pos) {
		steps.push_back(pos);
		right_pos = get_right(maze, pos, prev_dir);
		righthand.push_back(right_pos);

		next_dir = next_step(maze, pos, prev_dir);
		prev_dir = from_dir(next_dir);
		pos = get_next_pos(maze, pos, next_dir);
	}

	std::cout << "Size of the loop: " << steps.size() << std::endl;
	std::cout << "Half loop: " << steps.size() / 2.0 << std::endl;


	// PART 2
	// Go through loop and mark right element
	// print maze with in elements
	// mnually complete

	std::ofstream outfile ("day10.txt");
	if (!outfile.is_open()){
		std::cout << "Had problems opening file " << "day10.txt" << std::endl;
		return -1;
	}

	nrow = 0;
	for (auto row : maze) {
		for(auto it = row.begin(); it != row.end(); it++) {
			pos = {nrow, it - row.begin()};
			std::tie(i, j) = pos;
			int in_steps = std::count(steps.begin(), steps.end(), pos);
			int in_right = std::count(righthand.begin(), righthand.end(), pos);
			if (in_steps)
				outfile << 'P';
			else if (in_right)
				outfile << 'I';
			else
				outfile << '.';
		}
		nrow++;
		outfile << std::endl;
	}

	return 0;
}

