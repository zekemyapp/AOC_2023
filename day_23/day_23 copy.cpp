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

#define PART_TWO

typedef std::vector<int> History;
typedef std::tuple<int,int,int, History> Node;
std::vector<std::tuple<int,int,int, History>> frontier;
std::vector<int> solutions;
int nrow;
int ncol;
int endi, endj;
int64_t ohhigh = 0;

void solve (std::vector<std::string> maze, Node node)
{
    int i, j, steps;
    History h;
    std::tie(i, j, steps, h) = node;

    // already explored
    if (std::find(h.begin(), h.end(), (ncol*i)+j) != h.end())
        return;

    // Forest
    char v = maze.at(i).at(j);
    // if (v == '#')
    //     return;

    // Arrived to the end
    if (i == endi && j == endj) {
        if (steps > ohhigh) {
            ohhigh = steps;
            std::cout << "Current highest: " << ohhigh << std::endl;
        }
        solutions.push_back({steps});
    }

    // add to history
    h.push_back((ncol*i)+j);

#ifndef PART_TWO
    switch (v) {
    case 'v':
        if (i < nrow-1) {
            v = maze.at(i+1).at(j);
            if (v != '#')
                frontier.push_back({i+1, j, steps+1, h});
        }
        return;
    case '>':
        if (j < ncol-1) {
            v = maze.at(i).at(j+1);
            if (v != '#')
                frontier.push_back({i, j+1, steps+1, h});
            return;
        }
    }
#endif

    if (i < nrow-1) {
        v = maze.at(i+1).at(j);
        if (v != '#')
            frontier.push_back({i+1, j, steps+1, h});
    }
    if (i > 0) {
        v = maze.at(i-1).at(j);
        if (v != '#')
            frontier.push_back({i-1, j, steps+1, h});
    }
    if (j < ncol-2) {
        v = maze.at(i).at(j+1);
        if (v != '#')
            frontier.push_back({i, j+1, steps+1, h});
    }
    if (j > 1) {
        v = maze.at(i).at(j-1);
        if (v != '#')
            frontier.push_back({i, j-1, steps+1, h});
    }
}

int main (void) {
	std::cout << "Starting day 23" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

    std::vector<std::string> maze;

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
	}

    nrow = maze.size();
    ncol = maze.at(0).size();
    endi = nrow - 1;
    endj = ncol - 2;

    frontier.push_back({0, 1, 0, History()}); // Start at row 0, col 1 and 0 steps

    while(!frontier.empty()) {
        Node node = frontier.back();
        frontier.pop_back();
        solve(maze, node);
    }

    for (int steps : solutions) {
        if (steps > result_one)
            result_one = steps;
    }

    std::cout << "the result one is " << result_one << std::endl;

	return 0;
}

// 6086 TOO LOW
// 6158 TOO LOW