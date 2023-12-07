#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

#define USE_SAMPLE 0
#define PART_TWO 1

class Race {
    public:
        uint64_t T;
        uint64_t D;
        Race (uint64_t T, uint64_t D) : T(T), D(D) {}
};

int main (void)
{
    vector<Race> all_races;

#if USE_SAMPLE
    all_races.push_back(Race(7,9));
    all_races.push_back(Race(15,40));
    all_races.push_back(Race(30,200));
#elif PART_TWO
    all_races.push_back(Race(56717999, 334113513502430));
#else
    all_races.push_back(Race(56, 334));
    all_races.push_back(Race(71, 1135));
    all_races.push_back(Race(79, 1350));
    all_races.push_back(Race(99, 2430));
#endif

    int out = 1;
    for(Race race : all_races) {
        uint64_t a = race.T*race.T;
        uint64_t b = 4*race.D;
        double root = sqrt(a - b);
        uint64_t max = round((race.T + root) / 2.0);
        uint64_t min = round((race.T - root) / 2.0);

        uint64_t result = 0;
        for (uint64_t i=min; i<=max; i++) {
            uint64_t distance = (race.T-i)*i;
            if (distance > race.D)
                result += 1;
        }

#if PART_TWO
        out = result;
#else
        out *= result;
#endif
        cout << "The race " << race.T << " result is: " << result << endl;
    }

#if PART_TWO
    cout << "The response part two is: " << out << endl;
#else
    cout << "The response part one is: " << out << endl;
#endif

    return 0;
}