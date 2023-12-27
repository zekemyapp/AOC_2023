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

struct node {
	std::string name;
	std::vector<struct node*> conn;
};

int main (void) {
	std::cout << "Starting day 25" << std::endl;

	std::string filename = "sample.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

	std::unordered_map<std::string, struct node*> nodes;

    /* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);

		size_t div = file_content.find(":");
        std::string left = file_content.substr(0, div);
        std::string right = file_content.substr(div+1, std::string::npos);

		struct node* n;
		if (nodes.find(left) == nodes.end()) {
			n = new struct node;
			n->name = left;
			nodes[left] = n;
		} else {
			n = nodes[left];
		}

		std::stringstream ss(right);
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');

			struct node* n2;
			if (nodes.find(substr) == nodes.end()) {
				n2 = new struct node;
				n2->name = substr;
				nodes[substr] = n2;
			} else {
				n2 = nodes[substr];
			}

			n->conn.push_back(n2);
			n2->conn.push_back(n);
        }
	}

	struct node* A;
	std::unordered_map<std::string, struct node*> B;

	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		struct node* n_copy = new struct node;
		n_copy = it->second;
		B[it->first] = n_copy;
	}

	// A = B.begin()->second;
	// B.erase(B.begin());

	// while (B.size() > 2) {
	// 	std::set<struct node*> s( A->conn.begin(), A->conn.end() );
	// 	A->conn.assign( s.begin(), s.end() );
	// 	struct node* removed_node = A->conn.at(0);
	// 	A->conn.erase(A->conn.begin());

	// 	// Iterate over all thhe connections of removed_node
	// 	for (auto rn_connn_it = removed_node->conn.begin(); rn_connn_it != removed_node->conn.end(); rn_connn_it++) {
	// 		if ((*rn_connn_it)->name == A->name)
	// 			continue;

	// 		for (auto it2 = (*rn_connn_it)->conn.begin(); it2 != (*rn_connn_it)->conn.end(); it2++) {
	// 			if ((*it2)->name == A->name) {
	// 				(*rn_connn_it)->conn.erase(it2);
	// 				break;
	// 			}
	// 		}

	// 		for (auto it2 = (*rn_connn_it)->conn.begin(); it2 != (*rn_connn_it)->conn.end(); it2++) {
	// 			if ((*it2)->name == removed_node->name) {
	// 				(*rn_connn_it)->conn.erase(it2);
	// 				break;
	// 			}
	// 		}

	// 		(*rn_connn_it)->conn.push_back(A);
	// 		A->conn.push_back(*rn_connn_it);
	// 	}
		
	// 	B.erase(removed_node->name);
	// 	delete removed_node;
	// }

    std::cout << "the result one is " << result_one << std::endl;

	return 0;
}
