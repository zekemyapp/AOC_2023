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

class Brick {
    public:
        int x1, y1, z1;
        int x2, y2, z2;

        bool touch(int x, int y, int z) {
            if (x >= x1 && x <= x2 &&
                y >= y1 && y <= y2 &&
                z >= z1 && z <= z2)
                return true;
            return false;
        }

        bool operator==(Brick other) {
            if (x1 == other.x1 && x2 == other.x2 &&
                y1 == other.y1 && y2 == other.y2 &&
                z1 == other.z1 && z2 == other.z2)
                return true;
            return false;
        }
};

struct less_than_key
{
    inline bool operator() (const Brick& a, const Brick& b)
    {
        return (a.z1 < b.z1);
    }
};

void print_map(std::vector<Brick> bricks)
{
    // get highest x, y and z
    int max_x = 0;
    int max_y = 0;
    int max_z = 0;

    for (auto b : bricks) {
        if (b.x1 > max_x)
            max_x = b.x1;
        if (b.x2 > max_x)
            max_x = b.x2;

        if (b.y1 > max_y)
            max_y = b.y1;
        if (b.y2 > max_y)
            max_y = b.y2;

        if (b.z1 > max_z)
            max_z = b.z1;
        if (b.z2 > max_z)
            max_z = b.z2;
    }

    // Initialize map projections
    std::vector<std::string> xz(max_z+1, std::string(max_x+1, '.'));
    std::vector<std::string> yz(max_z+1, std::string(max_y+1, '.'));

    char c = 'Z';
    for (auto b : bricks) {
        c++;
        if (c == 'Z'+1)
            c = 'A';

        for (int i=b.z1; i<=b.z2; i++) {
            for (int j=b.x1; j<=b.x2; j++) {
                xz.at(i).at(j) = c;
            }
        }

        for (int i=b.z1; i<=b.z2; i++) {
            for (int j=b.y1; j<=b.y2; j++) {
                yz.at(i).at(j) = c;
            }
        }
    }

    std::cout << std::endl << "xz projection" << std::endl;
    for (auto it = xz.rbegin(); it < xz.rend(); it++) {
        std::cout << *it << std::endl;
    }

    std::cout << std::endl << "yz projection" << std::endl;
    for (auto it = yz.rbegin(); it < yz.rend(); it++) {
        std::cout << *it << std::endl;
    }

}

int count_safe(std::vector<Brick> bricks) {
    int are_safe = 0;
    for (int brick_idx=0; brick_idx<bricks.size(); brick_idx++) {
        Brick current_b = bricks.at(brick_idx);
        bool is_supporting = false;

        // Check every brick above the current one to see if it is being supported
        for (auto b : bricks) {
            // Ignore if its the same
            if (b == current_b)
                continue;

            // Ignore if the lowest square is not straight above current brick
            if (b.z1 != current_b.z2+1)
                continue;

            for (int i=current_b.x1; i<=current_b.x2; i++) {
                for (int j=current_b.y1; j<=current_b.y2; j++) {
                    if (b.touch(i, j, current_b.z2+1)) {
                        // b is touching current_brick. Gotta check if its the only one
                        int touch_counter = 0;

                        // Check every brick b2 below current b to check if its the only one
                        for (auto b2 : bricks) {
                            if (b2 == b)
                                continue;

                            if (b2.z2 != b.z1-1)
                                continue;

                            bool not_touched = true;
                            for (int i=b.x1; i<=b.x2 && not_touched; i++) {
                                for (int j=b.y1; j<=b.y2 && not_touched; j++) {
                                    if (b2.touch(i, j, b.z1-1)) {
                                        touch_counter++;
                                        not_touched= false;
                                    }
                                }
                            }
                        }

                        if (touch_counter < 2){
                            is_supporting = true;
                        }
                        break;
                    }
                }
            }
        }

        if (!is_supporting)
            are_safe++;
    }

    return are_safe;
}

int collapse_without(std::vector<Brick> bricks, int idx) {
    int result = 0;

    if (idx >= 0)
        bricks.erase(bricks.begin() + idx);

    for (int brick_idx=0; brick_idx<bricks.size(); brick_idx++) {
        Brick current_b = bricks.at(brick_idx);
        int low_z = 1;
        for (auto b : bricks) {
            // If its the same brick, ignore
            if (b == current_b)
                continue;

            // If the highest part of the brick is higher than the lowest part of the current brick. Ignore
            if (b.z2 >= current_b.z1)
                continue;

            // If the highest point is lower than the current lowest allowed. Ignore
            if (b.z2 < low_z)
                continue;

            for (int i=current_b.x1; i<=current_b.x2; i++) {
                for (int j=current_b.y1; j<=current_b.y2; j++) {
                    if (b.touch(i, j, b.z2)) {
                        low_z = b.z2 + 1;
                    }
                }
            }
        }
    
        if (low_z < current_b.z1) {
            int dist = current_b.z2 - current_b.z1;
            bricks.at(brick_idx).z1 = low_z;
            bricks.at(brick_idx).z2 = low_z + dist;
            result++;
        }
    }

    return result;
}

int main (void) {
	std::cout << "Starting day 22" << std::endl;

	std::string filename = "input.txt";
	std::ifstream input_file(filename);
	int64_t result_one = 0;
	int64_t result_two = 0;

    std::vector<Brick> bricks;

    /* Check if input file was correctly opened */
	if (!input_file.is_open()) {
		std::cout << "Had problems opening file " << filename << std::endl;
		return -1;
	}

	/* Read input into file */
	while (!input_file.eof()) {
		std::string file_content;
		getline(input_file, file_content);

        size_t div = file_content.find("~");
        std::string start = file_content.substr(0, div);
        std::string end = file_content.substr(div+1, std::string::npos);

        Brick brick;
        memset(&brick, -1, sizeof(brick));

        std::stringstream ss(start);
        while(ss.good()) {
            std::string substr;
            getline(ss, substr, ',');

            if(brick.x1 == -1)
                brick.x1 = atoi(substr.c_str());
            else if(brick.y1 == -1)
                brick.y1 = atoi(substr.c_str());
            else
                brick.z1 = atoi(substr.c_str());
        }

        ss = std::stringstream(end);
        while(ss.good()) {
            std::string substr;
            getline(ss, substr, ',');

            if(brick.x2 == -1)
                brick.x2 = atoi(substr.c_str());
            else if(brick.y2 == -1)
                brick.y2 = atoi(substr.c_str());
            else
                brick.z2 = atoi(substr.c_str());
        }

        bricks.push_back(brick);
	}

    // Sort bricks by altitude to optimize displacement
    std::sort(bricks.begin(), bricks.end(), less_than_key());

    bool modified = true;
    while (modified) {
        modified = false;
        for (int brick_idx=0; brick_idx<bricks.size(); brick_idx++) {
            Brick current_b = bricks.at(brick_idx);
            int low_z = 1;
            for (auto b : bricks) {
                // If its the same brick, ignore
                if (b == current_b)
                    continue;

                // If the highest part of the brick is higher than the lowest part of the current brick. Ignore
                if (b.z2 >= current_b.z1)
                    continue;

                // If the highest point is lower than the current lowest allowed. Ignore
                if (b.z2 < low_z)
                    continue;

                for (int i=current_b.x1; i<=current_b.x2; i++) {
                    for (int j=current_b.y1; j<=current_b.y2; j++) {
                        if (b.touch(i, j, b.z2)) {
                            low_z = b.z2 + 1;
                        }
                    }
                }
            }
        
            if (low_z < current_b.z1) {
                int dist = current_b.z2 - current_b.z1;
                bricks.at(brick_idx).z1 = low_z;
                bricks.at(brick_idx).z2 = low_z + dist;
                modified = true;
                // print_map(bricks);
                // std::cin.get();
            }
        }
    }

    // result_one = count_safe(bricks);
    // std::cout << "the result one is " << result_one << std::endl;


    for (int brick_idx=0; brick_idx<bricks.size(); brick_idx++) {
        int collapsed = collapse_without(bricks, brick_idx);
        // std::cout << collapsed << " collapsed" << std::endl;
        result_two+=collapsed;
    }

    std::cout << "the result two is " << result_two << std::endl;

	return 0;
}
