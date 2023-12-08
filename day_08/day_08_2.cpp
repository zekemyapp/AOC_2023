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

uint64_t calculate (string position, string instructions, map<std::string, Step> mMap) {
	uint64_t output = 0;
	string::iterator pc = instructions.begin();
 	while(position.at(2) != 'Z') {
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

	return output;
}

// Function to calculate the GCD (Greatest Common Divisor)
uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate the LCM (Least Common Multiple)
uint64_t lcm(uint64_t a, uint64_t b) {
    return (a * b) / gcd(a, b);
}

// Function to calculate the LCM of six numbers
uint64_t lcmOfSixNumbers(uint64_t num1, uint64_t num2, uint64_t num3, uint64_t num4, uint64_t num5, uint64_t num6) {
    uint64_t result = lcm(num1, lcm(num2, lcm(num3, lcm(num4, lcm(num5, num6)))));
    return result;
}

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
	std::cout << "Period: " << instructions.size() << endl;


	// uint64_t output = 0;
	string path1 = "JHA";
	string path2 = "NCA";
	string path3 = "MMA";
	string path4 = "AAA";
	string path5 = "TVA";
	string path6 = "DTA";
	// string::iterator pc = instructions.begin();

	uint64_t first = calculate(path1, instructions, mMap);
	uint64_t second = calculate(path2, instructions, mMap);
	uint64_t third = calculate(path3, instructions, mMap);
	uint64_t fourth = calculate(path4, instructions, mMap);
	uint64_t fifth = calculate(path5, instructions, mMap);
	uint64_t sixth = calculate(path6, instructions, mMap);

	uint64_t result = lcmOfSixNumbers(
		first, second, third, fourth, fifth, sixth);
	
	std::cout << "The response part two is: " << result << endl;

	return 0;
}