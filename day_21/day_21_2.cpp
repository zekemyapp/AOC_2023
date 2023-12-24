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
std::vector<std::tuple<int, int, int>> frontier;
size_t nrow;
size_t ncol;

void solve(std::vector<std::string> maze) {
    while(!frontier.empty()) {
        int i,j, steps;
        std::tie(i,j, steps) = frontier.back();
        frontier.pop_back();

        if (memo.find({i, j, steps}) != memo.end())
            continue;
        memo.insert({i, j, steps});
    
        int ri = i%(int)nrow;
        int rj = j%(int)ncol;

        if (ri < 0)
            ri += nrow;
        
        if (rj < 0)
            rj += ncol;

        char v = maze.at(ri).at(rj);
        if (v == '#')
            continue;

        if (steps == 0) {
            solution.insert({i,j});
            continue;
        }

        frontier.push_back({i+1, j, steps-1});
        frontier.push_back({i-1, j, steps-1});
        frontier.push_back({i, j+1, steps-1});
        frontier.push_back({i, j-1, steps-1});
    }
}


int main (void) {
	std::cout << "Starting day 21" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	uint64_t result_two = 0;

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

    nrow = maze.size();
    ncol = maze.at(0).size();

    // frontier.push_back({start_i, start_j, 65});
    // solve(maze);
    // result_one = solution.size();
    // std::cout << "the result 65 is " << result_one << std::endl;

    // frontier.push_back({start_i, start_j, 196});
    // solve(maze);
    // result_one = solution.size();
    // std::cout << "the result 196 is " << result_one << std::endl;

    // frontier.push_back({start_i, start_j, 327});
    // solve(maze);
    // result_one = solution.size();
    // std::cout << "the result 327 is " << result_one << std::endl;

    result_two = (uint64_t)  15567 * (uint64_t) 202300 * (uint64_t) 202300 + 3955 + (uint64_t) 15692 * (uint64_t) 202300;

	std::cout << "the result two is " << result_two << std::endl;
	return 0;
}

// 65 -> 3955
// 196 -> 35214
// 327 - > 97607
