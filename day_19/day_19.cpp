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


class Part {
	public:
		int64_t x, m, a, s;

	public:
		Part(int64_t x, int64_t m, int64_t a, int64_t s) :
			x(x), m(m), a(a), s(s) {}

		int64_t rating() {
			return x+m+a+s;
		}
};

class Condition {
	public:
		enum Type {LT, GT, TYPE_A, TYPE_R, DIRECT};
		enum Component {X, M, A, S};

	public:
		Type type;
		Component comp;
		int64_t val;

	public:
		std::string workflow;

	private:
		int64_t get_xmas_val (Part xmas) {
			switch (comp)
			{
			case X:
				return xmas.x;
			case M:
				return xmas.m;
			case A:
				return xmas.a;
			case S:
				return xmas.s;
			}

			return 0;
		}

		Component c2comp (char ccomp) {
			switch (ccomp)
			{
			case 'x':
				return X;
			case 'm':
				return M;
			case 'a':
				return A;
			case 's':
				return S;
			}

			return X;
		}

		Type c2type (char ctype) {
			switch (ctype)
			{
			case 'A':
				return TYPE_A;
			case 'R':
				return TYPE_R;
			case '<':
				return LT;
			case '>':
				return GT;
			}

			return DIRECT;
		}

	public:
		Condition (std::string type, std::string comp, std::string val, std::string workflow) {
			this->type = c2type(type.c_str()[0]);
			this->comp = c2comp(comp.c_str()[0]);
			this->val = atoll(val.c_str());
			this->workflow = workflow;
		}

		Condition (std::string type) {
			char ctype = type.c_str()[0];
			this->type = c2type(ctype);
			this->workflow = type;
		}

		/* Returns 1 if part accepted, -1 if rejected and 0 if no decission was made */
		int test_condition (Part xmas) {
			if (type == TYPE_A)
				return 1;

			if (type == TYPE_R)
				return -1;

			if (type == DIRECT)
				return -2;

			if (type == LT) {
				int64_t xmas_val = get_xmas_val(xmas);
				if (xmas_val < val)
					return -2;
				return 0;
			}

			if (type == GT) {
				int64_t xmas_val = get_xmas_val(xmas);
				if (xmas_val > val)
					return -2;
				return 0;
			}

			return 0;
		}

};

class Workflow;
typedef std::unordered_map<std::string, Workflow> WorkflowMap;

class Workflow {
	public:
		std::string id;
		std::vector<Condition> conds;

	public:
		Workflow(std::string id) : id(id) {};
		Workflow() {};

		void add_condition(Condition cond) {
			conds.push_back(cond);
		}

		int test_part(WorkflowMap works, Part xmas) {
			for (auto c : conds) {
				int ret = c.test_condition(xmas);

				if (ret == -2)
					return works[c.workflow].test_part(works, xmas);

				if (ret == 0)
					continue;

				return ret;
			}

			return 0;
		}
};

struct range {
	int64_t x_s, x_e;
	int64_t m_s, m_e;
	int64_t a_s, a_e;
	int64_t s_s, s_e;
};

int64_t solve_for(WorkflowMap works, Workflow w, struct range r, int ncond = 0)
{
	if (w.conds.size() <= ncond)
		return 0;

	Condition::Type type = w.conds[ncond].type;
	Condition::Component comp = w.conds[ncond].comp;
	int64_t val = w.conds[ncond].val;
	int64_t output = 0;

	if (type == Condition::Type::TYPE_R)
		return 0;
	
	if (type == Condition::Type::TYPE_A) {
		output = (r.x_e+1 - r.x_s) * (r.m_e+1 - r.m_s) * (r.a_e+1 - r.a_s) * (r.s_e+1 - r.s_s); 
		return output;
	}

	if (type == Condition::Type::DIRECT) {
		output = solve_for(works, works[w.conds[ncond].workflow], r);
		return output;
	}

	if (type == Condition::Type::LT) {
		struct range ra = {
			.x_s = r.x_s,
			.x_e = comp == Condition::Component::X ? val-1 : r.x_e,
			
			.m_s = r.m_s,
			.m_e = comp == Condition::Component::M ? val-1 : r.m_e,

			.a_s = r.a_s,
			.a_e = comp == Condition::Component::A ? val-1 : r.a_e,

			.s_s = r.s_s,
			.s_e = comp == Condition::Component::S ? val-1 : r.s_e,
		}; // ra makes the transition

		output += solve_for(works, works[w.conds[ncond].workflow], ra);

		struct range rb = {
			.x_s = comp == Condition::Component::X ? val : r.x_s,
			.x_e = r.x_e,
			
			.m_s = comp == Condition::Component::M ? val : r.m_s,
			.m_e = r.m_e,

			.a_s = comp == Condition::Component::A ? val : r.a_s,
			.a_e = r.a_e,

			.s_s = comp == Condition::Component::S ? val : r.s_s,
			.s_e = r.s_e,
		};

		output += solve_for(works, w, rb, ncond+1);

		return output;

	} else {
		struct range ra = {
			.x_s = r.x_s,
			.x_e = comp == Condition::Component::X ? val : r.x_e,
			
			.m_s = r.m_s,
			.m_e = comp == Condition::Component::M ? val : r.m_e,

			.a_s = r.a_s,
			.a_e = comp == Condition::Component::A ? val : r.a_e,

			.s_s = r.s_s,
			.s_e = comp == Condition::Component::S ? val : r.s_e,
		};

		output += solve_for(works, w, ra, ncond+1);

		struct range rb = {
			.x_s = comp == Condition::Component::X ? val+1 : r.x_s,
			.x_e = r.x_e,
			
			.m_s = comp == Condition::Component::M ? val+1 : r.m_s,
			.m_e = r.m_e,

			.a_s = comp == Condition::Component::A ? val+1 : r.a_s,
			.a_e = r.a_e,

			.s_s = comp == Condition::Component::S ? val+1 : r.s_s,
			.s_e = r.s_e,
		}; // rb makes the transition

		output += solve_for(works, works[w.conds[ncond].workflow], rb);

		return output;
	}

	return output;
}

int main (void) {
	std::cout << "Starting day 19" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

	std::vector<std::string> workflows_str;
	std::vector<std::string> parts_str;
	bool is_parts = false;

	WorkflowMap workflows;
	std::vector<Part> parts;

	/* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);

		if (is_parts) {
			parts_str.push_back(file_content);
			continue;
		}

		if (file_content.empty()) {
			is_parts = true;
			continue;
		}

		workflows_str.push_back(file_content);
	}

	Workflow w = Workflow("A");
	w.add_condition(Condition("A"));
	workflows[w.id] = w;
	w = Workflow("R");
	w.add_condition(Condition("R"));
	workflows[w.id] = w;

	for (auto line : workflows_str) {
		size_t div1 = line.find("{");
		size_t div2 = line.find("}");
		std::string id = line.substr(0, div1);
		std::string sconditions = line.substr(div1+1, div2-(div1+1));

		w = Workflow(id);

		std::stringstream ss(sconditions);
		while (ss.good()) {
			std::string substr;
			getline(ss, substr, ',');

			size_t op = substr.find_first_of("<>");
			if (op != std::string::npos) {
				size_t div3 = substr.find(":");
				std::string comp = substr.substr(0, 1);
				std::string oper = substr.substr(op, 1);
				std::string comp_val = substr.substr(op+1, div3-(op+1));
				std::string dest = substr.substr(div3+1, std::string::npos);

				w.add_condition(Condition(oper, comp, comp_val, dest));
				continue;
			}

			w.add_condition(Condition(substr));
		}

		workflows[w.id] = w;
	}

	for (auto line : parts_str) {
		size_t div1 = line.find("{");
		size_t div2 = line.find("}");
		std::string sparts = line.substr(div1+1, div2-(div1+1));

		int64_t x,m,a,s;

		std::stringstream ss(sparts);
		while (ss.good()) {
			std::string substr;
			getline(ss, substr, ',');

			char letter = substr.substr(0, 1).c_str()[0];
			int64_t value = atoll(substr.substr(2, std::string::npos).c_str());

			switch (letter)
			{
			case 'x':
				x = value;
			case 'm':
				m = value;
			case 'a':
				a = value;
			case 's':
				s = value;
			}
		}

		parts.push_back(Part(x,m,a,s));
	}

	for (auto p : parts) {
		if (workflows["in"].test_part(workflows, p) == 1) {
			result_one += p.rating();
		}
	}

	std::cout << "the result one is " << result_one << std::endl;

	struct range r = {
		.x_s = 1,
		.x_e = 4000,
		
		.m_s = 1,
		.m_e = 4000,

		.a_s = 1,
		.a_e = 4000,

		.s_s = 1,
		.s_e = 4000,
	};

	std::vector<struct range> ranges;
	ranges.push_back(r);

	result_two = solve_for(workflows, workflows["in"], r);
	
	std::cout << "the result two is " << result_two << std::endl;
	return 0;
}