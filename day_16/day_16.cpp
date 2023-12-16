#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_set>

enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

typedef std::tuple<int, int, Direction> Position;

void print_maze(std::vector<std::string> maze)
{
	for (auto s: maze) {
		std::cout << s << std::endl;
	}
}

void print_maze(std::vector<std::vector<char>> maze)
{
	for (auto s: maze) {
		for (auto c: s)
			std::cout << c;
		std::cout << std::endl;
	}
}

int count_energy(std::vector<std::vector<char>> maze)
{
	int out = 0;
	for (auto s: maze) {
		for (auto c: s)
			if (c == '#') out += 1;
	}

	return out;
}

// #include <bits/stdc++.h>
struct hashFunction
{
  size_t operator()(const Position&x) const
  {
    return std::get<0>(x) ^ std::get<1>(x) ^ std::get<2>(x);
  }
};
std::unordered_set<Position, hashFunction> memo;

void solve_maze(std::vector<std::string> maze,
	std::vector<std::vector<char>> &energy_maze,
	std::vector<Position> &rays)
{
	Position ray = rays.at(0);
	rays.erase(rays.begin());
	size_t ncols = maze.at(0).size();
	size_t nrows = maze.size();
	char v;

	if (memo.find(ray) != memo.end())
		return;
	memo.insert(ray);

	int i,j;
	Direction d;
	std::tie(i, j, d) = ray;

	if (j>=0 && j<ncols && i>=0 && i<nrows)
		energy_maze.at(i).at(j) = '#';

	switch (d) {
		case NORTH:
			if (i == 0)
				return;

			v = maze.at(i-1).at(j);
			if (v == '.' || v == '|') {
				rays.push_back({i-1, j, NORTH});
				return;
			}

			if (v == '-') {
				rays.push_back({i-1, j, EAST});
				rays.push_back({i-1, j, WEST});
				return;
			}

			if (v == '\\') {
				rays.push_back({i-1, j, WEST});
				return;
			}

			if (v == '/') {
				rays.push_back({i-1, j, EAST});
				return;
			}

			break;
		case EAST:
			if (j == ncols - 1)
				return;

			v = maze.at(i).at(j+1);
			if (v == '.' || v == '-') {
				rays.push_back({i, j+1, EAST});
				return;
			}

			if (v == '|') {
				rays.push_back({i, j+1, NORTH});
				rays.push_back({i, j+1, SOUTH});
				return;
			}

			if (v == '\\') {
				rays.push_back({i, j+1, SOUTH});
				return;
			}

			if (v == '/') {
				rays.push_back({i, j+1, NORTH});
				return;
			}

			break;
		case SOUTH:
			if (i == nrows - 1)
				return;

			v = maze.at(i+1).at(j);
			if (v == '.' || v == '|') {
				rays.push_back({i+1, j, SOUTH});
				return;
			}

			if (v == '-') {
				rays.push_back({i+1, j, EAST});
				rays.push_back({i+1, j, WEST});
				return;
			}

			if (v == '\\') {
				rays.push_back({i+1, j, EAST});
				return;
			}

			if (v == '/') {
				rays.push_back({i+1, j, WEST});
				return;
			}

			break;
		case WEST:
			if (j == 0)
				return;

			v = maze.at(i).at(j-1);
			if (v == '.' || v == '-') {
				rays.push_back({i, j-1, WEST});
				return;
			}

			if (v == '|') {
				rays.push_back({i, j-1, NORTH});
				rays.push_back({i, j-1, SOUTH});
				return;
			}

			if (v == '\\') {
				rays.push_back({i, j-1, NORTH});
				return;
			}

			if (v == '/') {
				rays.push_back({i, j-1, SOUTH});
				return;
			}

			break;
	}
}

int main (void) {
	std::cout << "Starting day 16" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	std::string file_content;
	uint64_t result_one = 0;
	int result_two = 0;

	std::vector<std::string> maze;
	std::vector<std::vector<char>> maze_energy;
	std::vector<Position> rays;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		getline(input_file, file_content);
		maze.push_back(file_content);
	}

	/* Fill empty maze */
	std::vector<char> empty_energy_row(maze.at(0).size());
	std::fill(empty_energy_row.begin(), empty_energy_row.end(), '.');
	for (auto l: maze)
		maze_energy.push_back(empty_energy_row);

	/* Initialize ray */
	rays.push_back(Position(0, -1, EAST));

	/* Solve */
	while(!rays.empty()) {
		solve_maze(maze, maze_energy, rays);
	}
	// print_maze(maze);
	// std::cout << std::endl;
	// print_maze(maze_energy);
	std::cout << "the result one is " << count_energy(maze_energy) << std::endl;

	for (int i=0; i<maze.size(); i++) {
		// LEFT
		maze_energy.clear();
		for (auto l: maze)
			maze_energy.push_back(empty_energy_row);
		memo.clear();
		rays.push_back(Position(i, -1, EAST));
		while(!rays.empty()) {
			solve_maze(maze, maze_energy, rays);
		}
		result_two = std::max(result_two, count_energy(maze_energy));

		// RIGHT
		maze_energy.clear();
		for (auto l: maze)
			maze_energy.push_back(empty_energy_row);
		memo.clear();
		rays.push_back(Position(i, maze.at(0).size(), WEST));
		while(!rays.empty()) {
			solve_maze(maze, maze_energy, rays);
		}
		result_two = std::max(result_two, count_energy(maze_energy));
	}

	for (int j=0; j<maze.at(0).size(); j++) {
		// UP
		maze_energy.clear();
		for (auto l: maze)
			maze_energy.push_back(empty_energy_row);
		memo.clear();
		rays.push_back(Position(-1, j, SOUTH));
		while(!rays.empty()) {
			solve_maze(maze, maze_energy, rays);
		}
		result_two = std::max(result_two, count_energy(maze_energy));

		// DOWN
		maze_energy.clear();
		for (auto l: maze)
			maze_energy.push_back(empty_energy_row);
		memo.clear();
		rays.push_back(Position(maze.size(), j, NORTH));
		while(!rays.empty()) {
			solve_maze(maze, maze_energy, rays);
		}
		result_two = std::max(result_two, count_energy(maze_energy));
	}

	std::cout << "the result two is " << result_two << std::endl;
	return 0;
}