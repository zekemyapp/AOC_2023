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


enum Direction {L, R};

class Step {
	public:
		string left;
		string right;
		Step(string left, string right) : left(left), right(right) {}
		Step(){}
};

int main (void)
{
	map<std::string, Step> mMap{};
	string instructions;

	string filename = "in_map.txt";
	ifstream mFile(filename);

	string ins_filename = "in_ins.txt";
	ifstream insFile(ins_filename);

	if (!mFile.is_open()) {
		std::cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	if (!insFile.is_open()) {
		std::cout << "Had problems opening file " << ins_filename << endl;
		return -1;
	}

	std::cout << "Starting day8 execution" << endl;

	string fileContent;
	vector<string> input;

	/* Read input and create and array for ease of use */
	while (!mFile.eof()) {
		getline(mFile, fileContent);
		input.push_back(fileContent);
	}

	for (string line : input) {
		size_t div = line.find_first_of(" ");
		string key = line.substr(0, div);

		div = line.find_first_of("(");
		string left = line.substr(div+1, 3);

		div = line.find_first_of(",");
		string right = line.substr(div+2, 3);

		mMap[key] = Step(left, right);
	}

	getline(insFile, instructions);
	std::cout << "Instructions: " << instructions << endl;

	int output = 0;
	string position = "AAA";
	string::iterator pc = instructions.begin();

	while(position.compare("ZZZ") != 0) {
		if (*pc == 'L') {
			position = mMap[position].left;
		} else {
			position = mMap[position].right;
		}

		output++;
		pc++;
		if (pc == instructions.end())
			pc = instructions.begin();
	}

	
	std::cout << "The response part one is: " << output << endl;

	return 0;
}