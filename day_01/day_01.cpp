#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define IS_SECOND_PART

void replace_text_digit (std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos;
	start_pos = str.find(from);

	while(start_pos != string::npos){
		str.replace(start_pos+1, 1, to);
		start_pos = str.find(from);
	}


}

int findDigits(string input)
{
	int out = 0;
	string::iterator it;
	int first, second;



#ifdef IS_SECOND_PART
	replace_text_digit(input, "one", "1");
	replace_text_digit(input, "two", "2");
	replace_text_digit(input, "three", "3");
	replace_text_digit(input, "four", "4");
	replace_text_digit(input, "five", "5");
	replace_text_digit(input, "six", "6");
	replace_text_digit(input, "seven", "7");
	replace_text_digit(input, "eight", "8");
	replace_text_digit(input, "nine", "9");
#endif


	for (it = input.begin(); it != input.end(); it++) {
        if (isdigit(*it)) {
			first = *it - '0';
			break;
		}
    }

	for (it = input.end() - 1; it >= input.begin(); it--) {
        if (isdigit(*it)) {
			second = *it - '0';
			break;
		}
    }

	out = first*10 + second;

	cout << "for string: " << input << "; first = " << first;
	cout << "; second = " << second << " coordinate is: " << out << endl;
	return out;
}

int main (void)
{
	string filename = "input.txt";
	ifstream mFile(filename);

	if (!mFile.is_open()) {
		cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	cout << "Starting day1 execution" << endl;

	string fileContent;
	int output = 0;
	while (!mFile.eof()) {
		mFile >> fileContent;
		output += findDigits(fileContent);
	}

	cout << "The response is: " << output << endl;

	return 0;
}