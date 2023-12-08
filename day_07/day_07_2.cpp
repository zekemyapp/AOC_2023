#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

using namespace std;

#define USE_SAMPLE 0
#define PART_TWO 1


enum card {
	J = 1,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	T,
	Q,
	K,
	A
};

enum handType {
	HIGH_CARD = 0,
	ONE_PAIR,
	TWO_PAIR,
	THREE_KIND,
	FULL_HOUSE,
	FOUR_KIND,
	FIVE_KIND
};

class Hand {
	public:
	enum card cards[5];
	enum handType type;
	int bid;
	string rep;
	Hand(enum card a, enum card b, enum card c, enum card d, enum card e, int bid, string rep) : bid(bid), rep(rep) {
		cards[0] = a;
		cards[1] = b;
		cards[2] = c;
		cards[3] = d;
		cards[4] = e;

		// this->sort();
		type = this->get_type();
	}
	private:
	void sort(){
		std::sort(cards, cards+5);
	}

	enum handType get_type(){
		std::set<enum card> unic;
		for (int i=0; i<5; i++)
			unic.insert(cards[i]);
		int jcount = count(cards, cards+5, J);

		if (unic.size() == 1)
			return FIVE_KIND;

		if (unic.size() == 2) {
			int repeated = count(cards, cards+5, *unic.begin());

			if (jcount > 0)
				return FIVE_KIND;

			if (repeated == 4 || repeated == 1)
				return FOUR_KIND;
			else
				return FULL_HOUSE;
		}

		if (unic.size() == 3) {
			std::set<enum card>::iterator it = unic.begin();
			int repeated1 = count(cards, cards+5, *it);
			it++;
			int repeated2 = count(cards, cards+5, *it);
			it++;
			int repeated3 = count(cards, cards+5, *it);

			if (repeated1 == 3 || repeated2 == 3 || repeated3 == 3) {
				if (jcount > 0)
					return FOUR_KIND;
				return THREE_KIND;
			}
			else {
				if (jcount == 2)
					return FOUR_KIND;
				if (jcount == 1)
					return FULL_HOUSE;
				return TWO_PAIR;
			}
		}

		if (unic.size() == 4) {
			if (jcount == 2 || jcount == 1)
				return THREE_KIND;
			return ONE_PAIR;
		}

		if (jcount == 1)
			return ONE_PAIR;

		return HIGH_CARD;
	}
};

void parse_cards (string cards, enum card* pcards)
{
	const char *ccards = cards.c_str();

	for (int i=0; i<5; i++) {
		switch (ccards[i]) {
		case '2':
			pcards[i] = TWO;
			break;
		case '3':
			pcards[i] = THREE;
			break;
		case '4':
			pcards[i] = FOUR;
			break;
		case '5':
			pcards[i] = FIVE;
			break;
		case '6':
			pcards[i] = SIX;
			break;
		case '7':
			pcards[i] = SEVEN;
			break;
		case '8':
			pcards[i] = EIGHT;
			break;
		case '9':
			pcards[i] = NINE;
			break;
		case 'T':
			pcards[i] = T;
			break;
		case 'J':
			pcards[i] = J;
			break;
		case 'Q':
			pcards[i] = Q;
			break;
		case 'K':
			pcards[i] = K;
			break;
		case 'A':
			pcards[i] = A;
			break;
		}
	}
}

// Given two values h1 and h2, return true when h1 must be closer to the top
bool sortingFunction(Hand h1, Hand h2)
{
	if (h1.type > h2.type)
		return true;

	if (h1.type < h2.type)
		return false;

	// Same type
	for (int i=0; i<=4; i++){
		if (h1.cards[i] > h2.cards[i])
			return true;
		if (h1.cards[i] < h2.cards[i])
			return false;
	}

	return true;
}

// 252055431 TOO LOW

int main (void)
{
	vector<Hand> hands;

	string filename = "input.txt";
	ifstream mFile(filename);

	if (!mFile.is_open()) {
		cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	cout << "Starting day7 execution" << endl;

	string fileContent;
	vector<string> input;

	/* Read input and create and array for ease of use */
	while (!mFile.eof()) {
		getline(mFile, fileContent);
		input.push_back(fileContent);
	}

	for (string line : input) {
		size_t div = line.find(" ");
		string cards = line.substr(0, div);
		string bid = line.substr(div+1, string::npos);
		enum card pcards[5];
		int nbid = atoi(bid.c_str());
		parse_cards(cards, pcards);

		hands.push_back(Hand(pcards[0], pcards[1], pcards[2], pcards[3], pcards[4], nbid, cards));
	}

	sort(hands.begin(), hands.end(), sortingFunction);

	int output = 0;
	int value = hands.size();
	for (Hand h : hands) {
		cout << "card:" << h.rep << " type=" << h.type << " " << h.bid << "*" << value << endl;
		output += h.bid * value;
		value--;
	}

	cout << "last value=" << value << endl;

	cout << "The response part two is: " << output << endl;

	return 0;
}