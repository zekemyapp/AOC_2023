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


uint64_t hpulse = 0;
uint64_t lpulse = 0;

uint64_t iterations = 0;
uint64_t f_st = 0;
uint64_t f_tn = 0;
uint64_t f_hh = 0;
uint64_t f_dt = 0;


class Node {
	public:
		enum Type {
			UNTYPED,
			FLIP_FLOP,
			NAND,
			BCASTER,
		} type;

		enum State {
			LOW,
			HIGH
		} state , last_state;

		std::string name;
		std::vector<Node*> inputs;
		std::vector<Node*> outputs;

		void add_input(Node *node) {
			inputs.push_back(node);
		}

		void add_output(Node *node) {
			outputs.push_back(node);
		}

		void on_pulse(State pulse, std::vector<std::tuple<Node *, Node::State>> &orders) {
			switch (type) {
			case UNTYPED:
				break;

			case BCASTER:
				state = pulse;
				for (Node *onode : outputs) {
					orders.push_back({onode, pulse});
					if(pulse == HIGH)
						hpulse++;
					else
						lpulse++;
				}
				break;

			case FLIP_FLOP:
				if(pulse == HIGH)
					return;

				if(state == HIGH)
					state = LOW;
				else
					state = HIGH;

				for (Node *onode : outputs) {
					orders.push_back({onode, state});
					if(state == HIGH)
						hpulse++;
					else
						lpulse++;
				}
				break;

			case NAND:
				bool all_high = true;
				for (Node *inode : inputs) {
					if(inode->state == LOW) {
						all_high = false;
						break;
					}
				}

				State out_pulse = all_high ? LOW : HIGH;
				state = out_pulse;

				for (Node *onode : outputs) {
					orders.push_back({onode, out_pulse});
					if(out_pulse == HIGH)
						hpulse++;
					else
						lpulse++;
				}

				if (name.compare("lv")==0) {
					for (Node *inode : inputs) {
						if (inode->name.compare("st") == 0 && inode->state == HIGH && f_st == 0)
							f_st = iterations;
						
						if (inode->name.compare("tn") == 0 && inode->state == HIGH && f_tn == 0)
							f_tn = iterations;

						if (inode->name.compare("hh") == 0 && inode->state == HIGH && f_hh == 0)
							f_hh = iterations;

						if (inode->name.compare("dt") == 0 && inode->state == HIGH && f_dt == 0)
							f_dt = iterations;
					}
				}

				break;
			}
		}
};

class Broadcaster : Node {
	public:
		Broadcaster(){
			type = BCASTER;
			state = LOW;
			name = "broadcaster";
		}
};

class FlipFlop : Node {
	public:
		FlipFlop(std::string n){
			type = FLIP_FLOP;
			state = LOW;
			name = n;
		}
};

class Nand : Node {
	public:
		Nand(std::string n){
			type = NAND;
			state = HIGH;
			name = n;
		}
};

class Untyped : Node {
	public:
		Untyped(std::string n){
			type = UNTYPED;
			state = HIGH;
			name = n;
		}
};

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
uint64_t lcmOf4Numbers(uint64_t num1, uint64_t num2, uint64_t num3, uint64_t num4) {
    uint64_t result = lcm(num1, lcm(num2, lcm(num3, num4)));
    return result;
}

int main (void) {
	std::cout << "Starting day 20" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

	std::vector<std::string> inputs;
	std::unordered_map<std::string, Node*> nodes;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);
		inputs.push_back(file_content);
	}

	// Create nodes
	for (auto line : inputs) {
		size_t div = line.find(";");
		std::string node_str = line.substr(0, div);

		if (node_str.compare("broadcaster")==0) {
			nodes["broadcaster"] = (Node*) new Broadcaster();
			continue;
		}

		std::string name = node_str.substr(1, std::string::npos);

		if (node_str.at(0) == '&') {
			nodes[name] = (Node*) new Nand(name);
			continue;
		}

		if (node_str.at(0) == '%') {
			nodes[name] = (Node*) new FlipFlop(name);
			continue;
		}
	}

	// Connect them
	for (auto line : inputs) {
		size_t div = line.find(";");
		std::string node_str = line.substr(0, div);
		std::string out_str = line.substr(div+1, std::string::npos);
		std::string name;

		if (node_str.compare("broadcaster")==0) {
			name = "broadcaster";
		} else {
			name = node_str.substr(1, std::string::npos);
		}

		Node *pnode = nodes[name];

		std::stringstream ss(out_str);
		while (ss.good()) {
			std::string substr;
			getline(ss, substr, ',');

			if (nodes.find(substr) == nodes.end()) {
				nodes[substr] = (Node *) new Untyped(substr);
			}

			Node *onode = nodes[substr];
			onode->add_input(pnode);
			pnode->add_output(onode);
		}
	}

	std::vector<std::tuple<Node *, Node::State>> orders;

	bool all_low = true;
	while (f_dt == 0 || f_hh == 0 || f_st == 0 || f_tn == 0) {
		iterations++;
		lpulse++;
		nodes["broadcaster"]->on_pulse(Node::LOW, orders);

		while(!orders.empty()) {
			Node *onode;
			Node::State pulse;
			std::tie(onode, pulse) = orders.at(0);
			orders.erase(orders.begin());

			onode->on_pulse(pulse, orders);
		}
	}

	std::cout << "high pulses " << hpulse << std::endl;
	std::cout << "low pulses " << lpulse << std::endl;
	result_one = hpulse*lpulse;

	std::cout << "the result one is " << result_one << std::endl;

	result_two = lcmOf4Numbers(f_st, f_tn, f_hh, f_dt);
	std::cout << "the result two is " << result_two << std::endl;
	return 0;
}