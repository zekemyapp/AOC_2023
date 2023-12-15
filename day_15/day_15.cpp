#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>


class Lense {
	public:
		std::string label;
		uint8_t fl;
		Lense (std::string label, uint8_t fl) : label(label), fl(fl) {}
};

class Box {
	public:
		std::vector<Lense> content;
		Box() {}

		void add_lense (Lense lense) {
			for (auto l=content.begin(); l != content.end(); l++) {
				if (l->label == lense.label) {
					*l = lense;
					return;
				}
			}

			content.push_back(lense);
		}

		void remove_lense (std::string label) {
			for (auto l=content.begin(); l != content.end(); l++) {
				if (l->label == label) {
					content.erase(l);
					return;
				}
			}
		}
};

uint8_t hash(std::string str)
{
	uint64_t current_value = 0;
	for (auto c : str) {
		uint8_t v = c;
		current_value += v;
		current_value *= 17;
		current_value %= 256;
	}

	return (uint8_t) current_value;
}

int main (void) {
	std::cout << "Starting day 15" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	std::string file_content;
	uint64_t result_one = 0;

	Box boxes[256];

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		getline(input_file, file_content);
	}

	std::stringstream ss(file_content);

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result_one += hash(substr);

		size_t div = substr.find_first_of("=-");
		std::string label = substr.substr(0, div);
		char operation = substr.at(div);

		uint8_t box_id = hash(label);
		auto box = &boxes[box_id];

		if (operation == '=') {
			uint8_t fl = substr.at(div + 1) - '0';
			box->add_lense(Lense(label, fl));
		} else {
			box->remove_lense(label);
		}
	}

	uint64_t result_two = 0;
	for(int i=0; i<256;i++) {
		Box box = boxes[i];
		for (int j = 0; j<box.content.size(); j++) {
			auto l = box.content.at(j);
			result_two += (1 + i) * (j + 1) * l.fl;
		}
	}

	std::cout << "the result one is " << result_one << std::endl;
	std::cout << "the result two is " << result_two << std::endl;
	return 0;
}