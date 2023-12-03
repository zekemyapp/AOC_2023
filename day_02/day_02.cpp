#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define IS_SECOND_PART


struct sample {
    int red;
    int green;
    int blue;
};

int getFullNumber(std::string str, size_t start)
{
    size_t end = str.find_first_not_of("0123456789");
    string result = str.substr(start, end - start);
    return atoi(result.c_str());
}

int getGameId (std::string str)
{
    /* The id is the first number to appear */
    size_t start_pos = str.find_first_of("0123456789");
    int id = getFullNumber(str, start_pos);
    return id;
}

int getSubsets(std::string str, std::string *array)
{
    size_t start_pos = str.find_first_of(":");

    size_t subset_start;
    size_t subset_end = start_pos;

    int count = 0;

    do {
        subset_start = subset_end + 1;
        subset_end = str.find_first_of(";", subset_start);
        array[count++] = str.substr(subset_start, subset_end-subset_start);
    } while(subset_end != string::npos);

    return count;
}

void getSample(std::string str, struct sample *sample)
{
    sample->red = 0;
    sample->green = 0;
    sample->blue = 0;

    size_t start;
    size_t end = 0;

    do {
        /* jumps either a ',' or the space before a subset */
        start = end + 1;
        end = str.find_first_of(",", start);
        string color_pair = str.substr(start, end-start);

        size_t n_start = color_pair.find_first_of("0123456789");
        int n = getFullNumber(color_pair, n_start);

        size_t is_red = color_pair.find("red");
        size_t is_green = color_pair.find("green");
        size_t is_blue = color_pair.find("blue");

        if (is_red != string::npos) {
            sample->red = n;
        } else if (is_green != string::npos) {
            sample->green = n;
        } else if (is_blue != string::npos) {
            sample->blue = n;
        }
    } while(end != string::npos);

    cout << "red: " << sample->red << "; green: " << sample->green << "; blue: " << sample->blue << endl;
}

bool is_posible (struct sample reference, struct sample sample)
{
    return (sample.red <= reference.red and sample.green <= reference.green and sample.blue <= reference.blue);
}

void update_min (struct sample sample, struct sample *min)
{
    if (sample.red > min->red)
        min->red = sample.red;

    if (sample.green > min->green)
        min->green = sample.green;

    if (sample.blue > min->blue)
        min->blue = sample.blue;
}

int main (void)
{
	string filename = "input.txt";
	ifstream mFile(filename);

    struct sample sample;
    struct sample reference = {
        .red = 12,
        .green = 13,
        .blue = 14
    };

	if (!mFile.is_open()) {
		cout << "Had problems opening file " << filename << endl;
		return -1;
	}

	cout << "Starting day2 execution" << endl;

	string fileContent;
    int id, n_subsets;
    string subsets[20];

	int output = 0;
    int output_2 = 0;
	while (!mFile.eof()) {
        getline(mFile, fileContent);
        cout << "line: " << fileContent << endl;

        id = getGameId(fileContent);
        n_subsets = getSubsets(fileContent, subsets);

        bool should_count  = true;
        struct sample min = {
            .red = 1,
            .green = 1,
            .blue = 1
        };

        for(int i = 0; i < n_subsets; i++) {
            cout << "subset: " << subsets[i] << endl;
            getSample(subsets[i], &sample);

            if (!is_posible(reference, sample)) {
                should_count = false;
            }

            update_min(sample, &min);
        }

        if(should_count)
            output += id;

        output_2 += min.red * min.green * min.blue;

        cout << "id=" << id << "; subsets=" << n_subsets << "; ";
        cout << endl;
	}

	cout << "The response part one is: " << output << endl;
    cout << "The response part two is: " << output_2 << endl;

	return 0;
}