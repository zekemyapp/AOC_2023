#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

// #define USE_SAMPLE
#define IS_SECOND_PART

#define CHECK_FILE_OPEN(FS, FN)\
	ifstream FS(FN);\
	if (!FS.is_open()) {\
		cout << "Had problems opening file " << FN << endl;\
		return -1;\
	}\

#define PARSE_RANGE_FILE(FS, MAP) 	\
	while (!FS.eof()) {		\
		getline(FS, fileContent);	\
						\
		ss.clear();			\
		ss << fileContent;		\
						\
		counter = 0;			\
		from_key = -1;			\
		from_val = -1;			\
		length = -1;			\
						\
		while (!ss.eof()) {		\
			ss >> temp;		\
			if (is_number(temp)) {			\
				uint64_t value = atoll(temp.c_str());	\
								\
				if (counter == 0)		\
					from_val = value;	\
				else if (counter == 1)		\
					from_key = value;	\
				else				\
					length = value;		\
				counter++;			\
			}					\
		}						\
								\
		if (counter == 3) {				\
			MAP.add(from_key, from_val, length);	\
		}						\
	}							\

class Seed {
	public:
		uint64_t id;
		Seed(uint64_t id) : id(id) {}
};

class Range {
	public:
		uint64_t from_key;
		uint64_t from_val;
		uint64_t length;

		Range(uint64_t from_key, uint64_t from_val, uint64_t length) :
			from_key(from_key),
			from_val(from_val),
			length(length) {};
};

class RangeHolder {
	private:
		vector<Range> map;
		// TODO: add method to find a key inside list of ranges

	public:
		RangeHolder() {}

		void add(uint64_t from_key, uint64_t from_val, uint64_t length) {
			map.push_back(Range(from_key, from_val, length));
		}

		void print_content() {
			for (Range n : map)
				cout << n.from_key << ' ';
			cout << endl;
		}

		uint64_t find_key(uint64_t key, uint64_t *valid_range) {
			for (Range r: map) {
				uint64_t to_key = r.from_key + r.length-1;
				if (key >= r.from_key && key <= to_key) {

					uint64_t dist_key_end = to_key - key;
					if (dist_key_end < *valid_range)
						*valid_range = dist_key_end;

					uint64_t dist_key_key = key - r.from_key;
					return r.from_val + dist_key_key;
				}
			}

			*valid_range = 0;	
			return key;
		}
};

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

vector<Seed> seeds;
RangeHolder soil;
RangeHolder fertilizer;
RangeHolder water;
RangeHolder light;
RangeHolder temperature;
RangeHolder humidity;
RangeHolder location;


uint64_t get_location (uint64_t id, uint64_t *valid_range)
{
	*valid_range = 0;
	uint64_t dummy;
	uint64_t ksoil = soil.find_key(id, valid_range);
	uint64_t kfertilizer = fertilizer.find_key(ksoil, &dummy);
	uint64_t kwater = water.find_key(kfertilizer,  &dummy);
	uint64_t klight = light.find_key(kwater,  &dummy);
	uint64_t ktemperature = temperature.find_key(klight,  &dummy);
	uint64_t khumidity = humidity.find_key(ktemperature,  &dummy);
	uint64_t klocation = location.find_key(khumidity,  &dummy);

	return klocation;
}

/*
 * Order:
 * seeds -> soil -> fertilizer -> water -> light
 * light -> temperature -> humidity -> location
*/

int main (void)
{
	int output = 0;
	int output_2 = 0;

#ifdef USE_SAMPLE
	string filename_seeds = "sample_seeds";
	string filename_soil = "sample_soil";
	string filename_fertilizer = "sample_fertilizer";
	string filename_water = "sample_water";
	string filename_light = "sample_light";
	string filename_temperature = "sample_temperature";
	string filename_humidity = "sample_humidity";
	string filename_location = "sample_location";
#else
	string filename_seeds = "in_seeds";
	string filename_soil = "in_soil";
	string filename_fertilizer = "in_fertilizer";
	string filename_water = "in_water";
	string filename_light = "in_light";
	string filename_temperature = "in_temperature";
	string filename_humidity = "in_humidity";
	string filename_location = "in_location";
#endif

	cout << "Starting day5 execution" << endl;
	string fileContent;

	CHECK_FILE_OPEN(fs_seeds, filename_seeds);
	CHECK_FILE_OPEN(fs_soil, filename_soil);
	CHECK_FILE_OPEN(fs_fertilizer, filename_fertilizer);
	CHECK_FILE_OPEN(fs_water, filename_water);
	CHECK_FILE_OPEN(fs_light, filename_light);
	CHECK_FILE_OPEN(fs_temperature, filename_temperature);
	CHECK_FILE_OPEN(fs_humidity, filename_humidity);
	CHECK_FILE_OPEN(fs_location, filename_location);


	stringstream ss;
	string temp;
	int counter;
	uint64_t from_key;
	uint64_t from_val;
	uint64_t length;

	/* Parse seeds */
	while (!fs_seeds.eof()) {
		getline(fs_seeds, fileContent);

		ss.clear();
		ss << fileContent;

		while (!ss.eof()) {
			ss >> temp;
			if (is_number(temp)) {
				uint64_t id = atoll(temp.c_str());
				seeds.push_back(Seed(id));
			}
		}
	}

	cout << "found seeds:" << endl;
	for (Seed s : seeds)
		cout << s.id << ' ';
	cout << endl;

	/* Parse soil */
	PARSE_RANGE_FILE(fs_soil, soil);
	PARSE_RANGE_FILE(fs_fertilizer, fertilizer);
	PARSE_RANGE_FILE(fs_water, water);
	PARSE_RANGE_FILE(fs_light, light);
	PARSE_RANGE_FILE(fs_temperature, temperature);
	PARSE_RANGE_FILE(fs_humidity, humidity);
	PARSE_RANGE_FILE(fs_location, location);

	cout << "found soils:" << endl;
	soil.print_content();

	cout << "found fertilizers:" << endl;
	fertilizer.print_content();

	cout << "found water:" << endl;
	water.print_content();

	uint64_t min_location = -1;

#ifdef IS_SECOND_PART
	for (uint64_t i=0; i<seeds.size()-1; i+=2) {
		Seed s = seeds.at(i);
		Seed s2 = seeds.at(i+1);

		uint64_t to_seed = s.id + s2.id - 1;
		cout << "DOING SEED: " << s.id << " TO SEED: " << to_seed << endl;

		uint64_t valid_range = 0;
		for(uint64_t j=s.id; j<=to_seed; j = j + 1 + valid_range){
			uint64_t location = get_location(j, &valid_range);
			if (location < min_location)
				min_location = location;
		}

	}

	cout << "The response part two is: " << min_location << endl;

	/* Optimize soils */
	// set<uint64_t> op_soils;
	// set<uint64_t> op_fertilizer;
	// set<uint64_t> op_water;
	// set<uint64_t> op_light;
	// set<uint64_t> op_temperature;
	// set<uint64_t> op_humidity;
	// set<uint64_t> op_location;
	// uint64_t valid_range = 0;
	// uint64_t current_th = 0;

	// for (uint64_t i=0; i<seeds.size()-1; i+=2) {
	// 	Seed s = seeds.at(i);
	// 	Seed s2 = seeds.at(i+1);

	// 	uint64_t from_seed = s.id; 
	// 	uint64_t to_seed = s.id + s2.id - 1;
	// 	cout << "DOING SEED: " << from_seed << " TO SEED: " << to_seed << endl;

	// 	for(uint64_t j=from_seed; j<=to_seed; j = j + 1 + valid_range) {
	// 		uint64_t ksoil = soil.find_key(j, &valid_range);
			// This doesnt work as it takes too much memory
	// 		op_soils.insert(ksoil);
	// 	}
	// }

	/*
	* Order:
	* seeds -> soil -> fertilizer -> water -> light
	* light -> temperature -> humidity -> location
	* 
	* 178159714 TOO HIGH
	*/

	// valid_range = 0;
	// current_th = 0;
	// for (uint64_t v : op_soils) {
	// 	if (v <= current_th)
	// 		continue;

	// 	uint64_t kfertilizer = fertilizer.find_key(v, &valid_range);
	// 	op_fertilizer.insert(kfertilizer);
	// 	current_th = v + valid_range;
	// }

	// valid_range = 0;
	// current_th = 0;
	// for (uint64_t v : op_fertilizer) {
	// 	if (v <= current_th)
	// 		continue;

	// 	uint64_t kwater = water.find_key(v, &valid_range);
	// 	op_water.insert(kwater);
	// 	current_th = v + valid_range;
	// }

	// valid_range = 0;
	// current_th = 0;
	// for (uint64_t v : op_water) {
	// 	if (v <= current_th)
	// 		continue;

	// 	uint64_t klight = light.find_key(v, &valid_range);
	// 	op_light.insert(klight);
	// 	current_th = v + valid_range;
	// }

	// valid_range = 0;
	// current_th = 0;
	// for (uint64_t v : op_light) {
	// 	if (v <= current_th)
	// 		continue;

	// 	uint64_t ktemperature = temperature.find_key(v, &valid_range);
	// 	op_temperature.insert(ktemperature);
	// 	current_th = v + valid_range;
	// }

	// valid_range = 0;
	// current_th = 0;
	// for (uint64_t v : op_temperature) {
	// 	if (v <= current_th)
	// 		continue;

	// 	uint64_t khumidity = humidity.find_key(v, &valid_range);
	// 	op_humidity.insert(khumidity);
	// 	current_th = v + valid_range;
	// }

	// valid_range = 0;
	// current_th = 0;
	// for (uint64_t v : op_humidity) {
	// 	if (v <= current_th)
	// 		continue;

	// 	uint64_t klocation = location.find_key(v, &valid_range);
	// 	op_location.insert(klocation);
	// 	current_th = v + valid_range;
	// }

	// for (uint64_t v : op_location) {
	// 	cout << "LOCATION: " << v << endl;
	// }

	// cout << "The response part two is: " << *op_location.begin() << endl;

#else
	for (int i=0; i<seeds.size(); i+=1) {
		Seed s = seeds.at(i);
		uint64_t location = get_location(s.id);
		cout << "DOING SEED: " << s.id << endl;
		if (location < min_location)
			min_location = location;
	}

	cout << "The response part one is: " << min_location << endl;
#endif

	

	return 0;
}