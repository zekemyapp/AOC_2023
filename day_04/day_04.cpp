#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define IS_SECOND_PART

class Card {
	private:
		int winning[10];
		int found[25];
		int n_win;
		int n_found;

		bool is_number(const std::string& s)
		{
			std::string::const_iterator it = s.begin();
			while (it != s.end() && std::isdigit(*it)) ++it;
			return !s.empty() && it == s.end();
		}

	public:
		int id;

		Card(){}
		Card(string str, int id) {
			size_t start_winning = str.find(":") + 1;
			size_t start_found  = str.find("|") + 1;
			stringstream ss;
			string temp;

			this->id = id;

			string winning_str = str.substr(start_winning, start_found - start_winning);
			cout << "using winning string: " << winning_str << endl;
			ss << winning_str;
			n_win = 0;
			while (!ss.eof()) {
				ss >> temp;
				if (is_number(temp)) {
					cout << "found numer: " << temp << endl;
					winning[n_win++] = atoi(temp.c_str());
				}
			}

			string found_str = str.substr(start_found, string::npos);
			cout << "using found string: " << found_str << endl;
			ss.clear();
			ss << found_str;
			n_found = 0;
			while (!ss.eof()) {
				ss >> temp;
				if (is_number(temp)) {
					cout << "found numer: " << temp << endl;
					found[n_found++] = atoi(temp.c_str());
				}
			}
		}

		int calculate_hits() {
			int i, j;
			int count = 0;

			for (i=0; i<n_win; i++) {
				int w = winning[i];

				for (j=0; j<n_found; j++) {
					if (found[j] == w)
						count++;
				}
			}

			return count;
		}

		int calculate_earnings() {
			int i, j;

			int count = 0;
			for (i=0; i<n_win; i++) {
				int w = winning[i];

				for (j=0; j<n_found; j++) {
					if (found[j] == w)
						count++;
				}
			}

			if (count == 0)
				return 0;

			return 1<<(count-1);
		}
};

class CardResolved {
	public:
		int id;
		int n;
		CardResolved(){}
		CardResolved(int id, int n) {
			this->id = id;
			this->n = n;
		}

		void copy(CardResolved card) {
			this->id = card.id;
			this->n = card.n;
		}
};

class CardHolder {
	public:
		CardResolved *content;
		size_t size;

	public:
		CardHolder() {
			content = nullptr;
			size = 0;
		}

		void push(CardResolved &card) {
			if (size == 0) {
				size++;
				content = new CardResolved[size];
				content[0].copy(card);
				return;
			}

			size++;
			CardResolved *new_content = new CardResolved[size];
			for (int i=0; i<size-1; i++){
				new_content[i].copy(content[i]);
			}

			new_content[size-1].copy(card);

			delete[] content;
			content = new_content;
		}

		CardResolved *pull(int id) {
			int first_found = -1;
			for (int i=0; i<size; i++) {
				if (content[i].id == id) {
					first_found = i;
					break;
				}
			}

			if (first_found == -1)
				return nullptr;

			CardResolved *output = new CardResolved();
			output->copy(content[first_found]);

			CardResolved *new_content = new CardResolved[size];
			for (int i=0; i<first_found; i++){
				new_content[i].copy(content[i]);
			}

			for (int i=first_found+1; i<size; i++) {
				new_content[i-1].copy(content[i]);
			}

			size--;
			delete[] content;
			content = new_content;

			return output;
		}

		int empty() {
			return (size == 0);
		}
};

int check_card(CardResolved *ref, CardResolved card) {
	if (card.n == 0)
		return 1;

	int result = 1;
	for(int i=card.id; i<card.id+card.n; i++) {
		result += check_card(ref, ref[i]);
	}

	return result;
}

int main (void)
{
	string filename = "input.txt";
	ifstream mFile(filename);

	if (!mFile.is_open()) {
		cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	cout << "Starting day4 execution" << endl;

	string fileContent;
	string *input = NULL;
	int line_count = 0;


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

	Card *card_holder = new Card[line_count];
	CardResolved *original_cards = new CardResolved[line_count];

	cout << "Input is: " << endl;
	for (int i=0; i<line_count; i++) {
		cout << input[i] << endl;
	}

	for (int i=0; i<line_count; i++) {
		string c_line = input[i];
		size_t start;
		size_t end;

		cout << "processing line: " << i << endl;
		card_holder[i] = Card(c_line, i+1);
	}

	for (int i=0; i<line_count; i++) {
		int earn = card_holder[i].calculate_earnings();
		cout << "calculated earning: " << earn << endl;
		output += earn;

		original_cards[i].id = i+1;
		original_cards[i].n = card_holder[i].calculate_hits();

		cout << "added original card: id=" << original_cards[i].id  << "; n=" << original_cards[i].n << endl;
	}

	cout << "The response part one is: " << output << endl;
	int output_2 = 0;

	for(int i=0; i<line_count; i++) {
		output_2 += check_card(original_cards, original_cards[i]);
	}

	cout << "The response part two is: " << output_2 << endl;

	delete[] card_holder;
	delete[] input;
	return 0;
}