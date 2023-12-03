#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define IS_SECOND_PART

struct gear {
	int row;
	int col;
	int count;
	int ratio;
};

class Index {
	public:
		int row;
		int col;
		Index(int row=0, int col=0) {
			this->row = row;
			this->col = col;
		}

		bool compare(int row, int col) {
			if (this->row == row and this->col == col)
				return true;
			return false;
		}
};

class IndexArray {
	private:
		size_t size;
		bool is_symbol(char c) {
			return (c != '.' && !isdigit(c));
		}
		bool is_gear(char c)
		{
			return (c == '*');
		}

	public:
		Index *content;
		IndexArray() {
			content = nullptr;
			size = 0;
		}

		bool contains(int row, int col) {
			if (size == 0)
				return false;

			for (int i=0; i<size; i++) {
				if (content[i].compare(row, col))
					return true;
			}

			return false;
		}

		void clear() {
			delete[] content;
			content = nullptr;
			size = 0;
		}

		void add(int row, int col) {
			if (contains(row, col))
				return;

			if (size == 0) {
				size++;
				content = new Index[size];
				content[0].row = row;
				content[0].col = col;
				return;
			}

			size++;
			Index *new_content = new Index[size];
			for (int i=0; i<size-1; i++){
				new_content[i].row = content[i].row;
				new_content[i].col = content[i].col;
			}

			new_content[size-1].row = row;
			new_content[size-1].col = col;

			delete[] content;
			content = new_content;
		}

		bool check_all(std::string *str, int n, struct gear *gears, size_t n_gears) {
			for (int i=0; i<size; i++) {
				const char* c_row = str[content[i].row].c_str();

				if (is_gear(c_row[content[i].col])) {
					for (int gear = 0; gear<n_gears; gear++) {
						if (gears[gear].row == content[i].row && gears[gear].col == content[i].col) {
							gears[gear].count += 1;
							if (gears[gear].count == 1)
								gears[gear].ratio = n;
							else if (gears[gear].count == 2)
								gears[gear].ratio *= n;
							break;
						}
					}
				}

				if (is_symbol(c_row[content[i].col])) {
					return true;
				}
			}
			return false;
		}
};

void fill_indexes(std::string *str, size_t n_lines, size_t row, size_t col, IndexArray &index_list)
{
	size_t n_cols = str[0].length();

	if (row == 0) {
		if (col == 0) {
			index_list.add(row, col+1);
			index_list.add(row+1, col);
			index_list.add(row+1, col+1);

		} else if (col == n_cols-1) {
			index_list.add(row, col-1);
			index_list.add(row+1, col);
			index_list.add(row+1, col-1);

		} else {
			index_list.add(row, col+1);
			index_list.add(row, col-1);
			index_list.add(row+1, col);
			index_list.add(row+1, col+1);
			index_list.add(row+1, col-1);
		}

	} else if (row == n_lines-1){
		if (col == 0) {
			index_list.add(row, col+1);
			index_list.add(row-1, col);
			index_list.add(row-1, col+1);

		} else if (col == n_cols-1) {
			index_list.add(row, col-1);
			index_list.add(row-1, col);
			index_list.add(row-1, col-1);

		} else {
			index_list.add(row, col+1);
			index_list.add(row, col-1);
			index_list.add(row-1, col);
			index_list.add(row-1, col+1);
			index_list.add(row-1, col-1);
		}

	} else {
		if (col == 0) {
			index_list.add(row, col+1);
			index_list.add(row+1, col);
			index_list.add(row+1, col+1);
			index_list.add(row-1, col);
			index_list.add(row-1, col+1);

		} else if (col == n_cols-1) {
			index_list.add(row, col-1);
			index_list.add(row+1, col);
			index_list.add(row+1, col-1);
			index_list.add(row-1, col);
			index_list.add(row-1, col-1);

		} else {
			index_list.add(row, col+1);
			index_list.add(row, col-1);
			index_list.add(row+1, col);
			index_list.add(row+1, col+1);
			index_list.add(row+1, col-1);
			index_list.add(row-1, col);
			index_list.add(row-1, col+1);
			index_list.add(row-1, col-1);
		}
	}

}

bool is_gear(char c)
{
	return (c == '*');
}


size_t count_gears (string *input, int line_count, struct gear **gears) {
	size_t n_cols = input[0].length();
	int gears_count = 0;

	for (int i=0; i<line_count; i++) {
		const char *r = input[i].c_str();

		for (int j=0; j<n_cols; j++) {
			if (is_gear(r[j]))
				gears_count += 1;
		}
	}

	struct gear *new_gears = new struct gear[gears_count];

	int new_count = 0;
	for (int i=0; i<line_count; i++) {
		const char *r = input[i].c_str();

		for (int j=0; j<n_cols; j++) {
			if (is_gear(r[j])) {
				new_gears[new_count].row = i;
				new_gears[new_count].col = j;
				new_gears[new_count].count = 0;
				new_gears[new_count].ratio = 0;
				new_count++;
			}
		}
	}

	*gears = new_gears;
	return gears_count;
}

int main (void)
{
	string filename = "input.txt";
	ifstream mFile(filename);

	if (!mFile.is_open()) {
		cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	cout << "Starting day3 execution" << endl;

	string fileContent;
	string *input = NULL;
	int line_count = 0;
	IndexArray index_array;

	int output = 0;

	/* Read input and create and array for ease of use */
	while (!mFile.eof()) {
		getline(mFile, fileContent);
		line_count++;
		string *new_input = new string[line_count];
		for (int i=0; i<line_count-1; i++) {
			new_input[i] = input[i];
		}

		new_input[line_count-1] = fileContent;
		delete[] input;
		input = new_input;
	}

	cout << "Input is: " << endl;
	for (int i=0; i<line_count; i++) {
		cout << input[i] << endl;
	}

	struct gear *gears;
	size_t n_gears = count_gears(input, line_count, &gears);

	cout << "count gears=" << n_gears << endl;
	for(int gear=0; gear<n_gears; gear++) {
		cout << "gear at row=" << gears[gear].row << " col=" << gears[gear].col << endl;
	}

	for (int i=0; i<line_count; i++) {
		string c_line = input[i];
		size_t start;
		size_t end;

		cout << "processing line: " << i << endl;

		start = c_line.find_first_of("0123456789");
		while(start != string::npos) {
			end = c_line.find_first_not_of("0123456789", start);
			/* Calcule substring size and take into account last position */
			size_t n_len = (end == string::npos ? c_line.length() : end) - start;
			string result = c_line.substr(start, n_len);
			int n = atoi(result.c_str());

			// cout << "number: " << n << endl;

			index_array.clear();
			for (size_t j = start; j < start+n_len; j++) {
				fill_indexes(input, line_count, i, j, index_array);
			}

			if (index_array.check_all(input, n, gears, n_gears)) {
				output += n;
				// cout << "adding... " << n << endl;
			}

			start = c_line.find_first_of("0123456789", end);
		}
	}

	cout << "The response part one is: " << output << endl;

	int output_2 = 0;

	for(int gear=0; gear<n_gears; gear++) {
		if(gears[gear].count == 2) {
			output_2 += gears[gear].ratio;
			cout << "counting gear at row=" << gears[gear].row << " col=" << gears[gear].col << " and ratio=" << gears[gear].ratio << endl;
		} else {
			cout << "gear at row=" << gears[gear].row << " col=" << gears[gear].col << " had count=" << gears[gear].count << endl;
		}
	}

	cout << "The response part two is: " << output_2 << endl;


	delete[] input;
	return 0;
}