#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>

#include <stdlib.h>

#define PART2

#ifdef PART2
#define MAX_STRAIGHT_LINE 10
#else
#define MAX_STRAIGHT_LINE 3
#endif

enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

typedef std::tuple<int, int, Direction, int> Position;

class Node {
	private:
		int hdist (int i, int j, int dest_i, int dest_j) {
			return abs(dest_i - i) + abs (dest_j - j);
		}
	public:
		int i;
		int j;
		Direction d;
		int score;
		int h;
		int fsteps;

		Node(int i, int j, int dest_i, int dest_j, Direction d, int score, int fsteps=1) :
			i(i), j(j), d(d), score(score), fsteps(fsteps) {
				h = hdist(i,j,dest_i, dest_j);
			}

		bool operator==(const Node& other) const {
			return (i == other.i && j == other.j && d == other.d && score == other.score && h == other.h && fsteps == other.fsteps);
		}
};

bool cmp(Node a, Node b) {
    if (a == b)
		return false;

	int a_t = a.score + a.h;
	int b_t = b.score + b.h;

	return (a_t <= b_t);
}

struct hashFunction
{
  size_t operator()(const Position&x) const
  {
    return std::get<0>(x) ^ std::get<1>(x) ^ std::get<2>(x) ^ std::get<3>(x);
  }
};
std::unordered_set<Position, hashFunction> already_visited;

void generate_nodes(std::set<Node, decltype(&cmp)> &frontier, Node node, int dest_i, int dest_j)
{
	bool can_turn = true;
#ifdef PART2
	can_turn = !(node.fsteps < 4);
#endif


	switch (node.d) {
		case NORTH:
			frontier.insert(Node(node.i-1, node.j, dest_i, dest_j, NORTH, node.score, node.fsteps+1));
			if (!can_turn)
				break;
			frontier.insert(Node(node.i, node.j-1, dest_i, dest_j, WEST, node.score));
			frontier.insert(Node(node.i, node.j+1, dest_i, dest_j, EAST, node.score));
			break;
		case EAST:
			frontier.insert(Node(node.i, node.j+1, dest_i, dest_j, EAST, node.score, node.fsteps+1));
			if (!can_turn)
				break;
			frontier.insert(Node(node.i-1, node.j, dest_i, dest_j, NORTH, node.score));
			frontier.insert(Node(node.i+1, node.j, dest_i, dest_j, SOUTH, node.score));
			break;
		case SOUTH:
			frontier.insert(Node(node.i+1, node.j, dest_i, dest_j, SOUTH, node.score, node.fsteps+1));
			if (!can_turn)
				break;
			frontier.insert(Node(node.i, node.j-1, dest_i, dest_j, WEST, node.score));
			frontier.insert(Node(node.i, node.j+1, dest_i, dest_j, EAST, node.score));
			break;
		case WEST:
			frontier.insert(Node(node.i, node.j-1, dest_i, dest_j, WEST, node.score, node.fsteps+1));
			if (!can_turn)
				break;
			frontier.insert(Node(node.i-1, node.j, dest_i, dest_j, NORTH, node.score));
			frontier.insert(Node(node.i+1, node.j, dest_i, dest_j, SOUTH, node.score));
			break;
	}
}

int main (void) {
	std::cout << "Starting day 17" << std::endl;

	std::string filename = "sample.txt";
	std::ifstream input_file(filename);
	uint64_t result_one = 0;
	uint64_t result_two = 0;

	std::vector<std::string> maze;
	std::set<Node, decltype(cmp)*> frontier(cmp);

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

	size_t nrows = maze.size();
	size_t ncols = maze.at(0).size();
	int destination_i = nrows-1;
	int destination_j = ncols-1;

	Node node = Node(0, 0, destination_i, destination_j, SOUTH, -(maze.at(0).at(0) - '0'));
	frontier.insert(node);
#ifdef PART2
	node = Node(0, 0, destination_i, destination_j, EAST, -(maze.at(0).at(0) - '0'));
	frontier.insert(node);
#endif


	while(!frontier.empty()) {
		node = *frontier.begin();
		frontier.erase(frontier.begin());
		if (node.i < 0 || node.i > nrows-1 || node.j < 0 || node.j > ncols-1)
			continue;

		if (node.fsteps > MAX_STRAIGHT_LINE)
			continue;

		if (already_visited.find(Position(node.i, node.j, node.d, node.fsteps)) != already_visited.end())
			continue;
		already_visited.insert(Position(node.i, node.j, node.d, node.fsteps));

		int heat = maze.at(node.i).at(node.j) - '0';
		node.score += heat;

#ifdef PART2
		if (node.i == destination_i && node.j == destination_j && node.fsteps >= 4){
#else
		if (node.i == destination_i && node.j == destination_j){
#endif
			result_one = node.score;
			break;
 		}

		generate_nodes(frontier, node, destination_i, destination_j);
	}

#ifdef PART2
	std::cout << "the result two is " << result_one << std::endl;
#else
	std::cout << "the result one is " << result_one << std::endl;
#endif
	return 0;
}