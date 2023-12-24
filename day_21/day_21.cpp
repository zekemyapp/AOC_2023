#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <stdlib.h>

#define REMAINING_STEPS 6

struct hashFunction
{
  size_t operator()(const std::tuple<int, int>&x) const
  {
    return std::get<0>(x) ^ std::get<1>(x);
  }
};
std::unordered_set<std::tuple<int, int>, hashFunction> solution;

struct hashFunction2
{
  size_t operator()(const std::tuple<int, int, int>&x) const
  {
    return std::get<0>(x) ^ std::get<1>(x) ^ std::get<2>(x);
  }
};
std::unordered_set<std::tuple<int, int, int>, hashFunction2> memo;

void solve(std::vector<std::string> maze, int i, int j, int steps=REMAINING_STEPS)
{
    size_t nrow = maze.size();
    size_t ncol = maze.at(0).size();

    if (i < 0 || i >= nrow || j < 0 || j >= ncol)
        return;

    if (memo.find({i, j, steps}) != memo.end())
        return;

    memo.insert({i, j, steps});

    char v = maze.at(i).at(j);

    if (v == '#')
        return;

    if (steps == 0) {
        solution.insert({i,j});
        return;
    }

    solve(maze, i+1, j, steps-1);
    solve(maze, i-1, j, steps-1);
    solve(maze, i, j+1, steps-1);
    solve(maze, i, j-1, steps-1);
}

int main (void) {
	std::cout << "Starting day 20" << std::endl;

	std::string filename = "sample.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

    std::vector<std::string> maze;
    int start_i, start_j;

    /* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);
        maze.push_back(file_content);

        size_t pos =  file_content.find("S");
        if (pos != std::string::npos) {
            start_i = maze.size()-1;
            start_j = pos;
        }
	}

    solve(maze, start_i, start_j);
    result_one = solution.size();

    std::cout << "the result one is " << result_one << std::endl;
	std::cout << "the result two is " << result_two << std::endl;
	return 0;
}