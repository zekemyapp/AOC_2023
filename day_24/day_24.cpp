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

#define IS_SAMPLE

#ifdef IS_SAMPLE
#define L_LIMIT 7
#define H_LIMIT 27
#else
#define L_LIMIT 200000000000000
#define H_LIMIT 400000000000000
#endif

class Path {
    public:
        int64_t x;
        int64_t y;
        int64_t z;
        int64_t vx;
        int64_t vy;
        int64_t vz;
};

bool are_paralel (Path A, Path B)
{
    int64_t cross = A.vx * B.vy - B.vx * A.vy;
    return (cross == 0);
}

bool check_intersect(Path A, Path B)
{
    if (are_paralel(A, B))
        return false;

    float Vy1dVx1 = (float) A.vy / (float) A.vx;
    float t2num = Vy1dVx1 * (B.x - A.x) - (B.y - A.y);
    float t2den = B.vy - (Vy1dVx1 * B.vx);
    float t2 = t2num / t2den;

    float t1num = (B.x - A.x) + (t2 * B.vx);
    float t1 = t1num / A.vx;

    if (t2 < 0 || t1 < 0)
        return false;

    int64_t x = A.x + t1 * A.vx;
    int64_t y = A.y + t1 * A.vy;

    if (x >= L_LIMIT && x <= H_LIMIT && y >= L_LIMIT && y <= H_LIMIT)
        return true;

    return false;
}

bool check_intersect_3D(Path A, Path B)
{
    float Vy1dVx1 = (float) A.vy / (float) A.vx;
    float t2num = Vy1dVx1 * (B.x - A.x) - (B.y - A.y);
    float t2den = B.vy - (Vy1dVx1 * B.vx);

    if (Vy1dVx1 == 0)
        return false;

    float t2 = t2num / t2den;
    float t1num = (B.x - A.x) + (t2 * B.vx);
    float t1 = t1num / A.vx;

    if (t2 < 0 || t1 < 0)
        return false;

    int64_t x1 = A.x + t1 * A.vx;
    int64_t y1 = A.y + t1 * A.vy;
    int64_t z1 = A.z + t1 * A.vz;

    int64_t x2 = B.x + t1 * B.vx;
    int64_t y2 = B.y + t1 * B.vy;
    int64_t z2 = B.z + t1 * B.vz;

    if (x1 == x2 && y1 == y2 && z1 == z2)
        return true;

    return false;
}

int main (void) {
	std::cout << "Starting day 24" << std::endl;

	std::string filename = "sample.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

    std::vector<Path> allpaths;

    /* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);

        size_t div = file_content.find("@");
        std::string posstr = file_content.substr(0, div);
        std::string velstr = file_content.substr(div+1, std::string::npos);

        Path path;
        int count=0;

        std::stringstream ss(posstr);
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');

            switch(count) {
                case 0:
                    path.x = atoll(substr.c_str());
                    break;
                case 1:
                    path.y = atoll(substr.c_str());
                    break;
                case 2:
                    path.z = atoll(substr.c_str());
                    break;
            }
            count++;
        }

        ss = std::stringstream(velstr);
        count = 0;
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');

            switch(count) {
                case 0:
                    path.vx = atoll(substr.c_str());
                    break;
                case 1:
                    path.vy = atoll(substr.c_str());
                    break;
                case 2:
                    path.vz = atoll(substr.c_str());
                    break;
            }
            count++;
        }

        allpaths.push_back(path);
	}

    int npath = allpaths.size();
    for (int i=0; i<npath-1; i++) {
        Path A = allpaths[i];
        for (int j=i+1; j<npath; j++) {
            Path B = allpaths[j];
            if (check_intersect(A, B))
                result_one++;
        }
    }
    std::cout << "the result one is " << result_one << std::endl;

	return 0;
}
