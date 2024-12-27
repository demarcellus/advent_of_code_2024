#include "day20.hpp"
		
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <queue>
#include <unordered_set>

#include <algorithm>
#include <map>

#include <climits>

bool Day20::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<char>> map;
	
	readInput(&map);
	
	// print
	std::cout << "MAP:\n";
	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			std::cout << map[y][x];
		}
		std::cout << " ...\n";
	}
	std::cout << "...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = saveAtLeastLimitPicosecondsCheats(map, 2, 100);
	std::cout << ">>> UNDER 100ps WITH 2ps CHEAT COUNT: [" << result << "]\n";
	
	int result2 = saveAtLeastLimitPicosecondsCheats(map, 20, 100);
	std::cout << ">>> UNDER 100ps WITH 20ps CHEAT COUNT: [" << result2 << "]\n";
					
	return true;
}

bool Day20::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = saveAtLeastLimitPicosecondsCheats_test();
	if(ok) ok = saveAtLeastLimitPicosecondsCheats_test2();

	return ok;
}

/**
 * CODE
 */
int Day20::saveAtLeastLimitPicosecondsCheats(const std::vector<std::vector<char>>& map, const int cheat_length, const int limit)
{
	// output var
	int count = 0;
	
	// get path (there are NO branches)
	// for each point on path, check if there are cheats with at least limit profit with 2 picoseconds
	// there are NO branches so we can just check path positions against each other
	const int picoseconds_cheat = cheat_length;
	
	const std::vector<std::pair<int,int>> path = getPath(map);
	
	std::map<int ,int> cheats_sorted;
	for(size_t index = 0; index < path.size(); index++)
	{
		// get current position
		const auto position = path[index];
		const int  cost_at_position = index;
		
		// get locations withing X picoseconds distance
		const auto locs = getLocationsInMapFromLocationWithDistance(map, position, picoseconds_cheat);
		
		// check if they have "profit"
		for(auto& loc : locs)
		{
			const auto it = std::find(path.begin(), path.end(), loc);	   // limit search in ordered list --> filters low-picosecond cheats out
			if(it == path.end()) continue;
			
			const int cost_at_loc = std::distance(path.begin(), it);
			const int distance_traveled_with_cheat = std::abs(position.first - loc.first) + std::abs(position.second - loc.second);
			
			const int savings = (cost_at_loc - cost_at_position) - distance_traveled_with_cheat;       // savings = difference - distance traveled in cheat
						
			if(savings < picoseconds_cheat) continue;
			
			cheats_sorted[savings]++;
		}
	}

	//std::cout << "cheats sorted:\n";
	for(const auto& [ps, num] : cheats_sorted)
	{
		//std::cout << "[" << num << " --> " << ps << "]\n";
		
		if(ps >= limit) count += num;
	}
	//std::cout << "count [" << count << "]\n";
		
	return count;
}

std::pair<int,int> Day20::findLocation(const std::vector<std::vector<char>>& map, const char c)
{
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			if(map[y][x] == c) return {x,y};			
		}
	}
	
	return {-1, -1};
}

std::vector<std::pair<int,int>> Day20::getPath(const std::vector<std::vector<char>>& map)
{
	const std::pair<int, int> start = findLocation(map, 'S');
	const std::pair<int, int> end   = findLocation(map, 'E');
	
	std::pair<int,int> pos = start;
	std::vector<std::pair<int,int>> path;
	
	while(true)
	{	
		// temp position
		const std::pair<int,int> current = pos;

		// save position
		path.push_back(current);
		
		// check end condition
		if(current == end) break;
		
		// find next position
		const std::vector<std::pair<int,int>> dirs = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		for(auto& dir : dirs)
		{
			const std::pair<int,int> next = {current.first + dir.first, current.second + dir.second};
						
			if(std::find(path.begin(), path.end(), next) != path.end())
			{
				// already part of path
				continue;
			}
			else if(map[next.second][next.first] == '#')
			{
				// in a wall
				continue;
			}
			else
			{
				pos = next;
			}
		}
	}
	
	return path;	
}

std::vector<std::pair<int,int>> Day20::getLocationsInMapFromLocationWithDistance(const std::vector<std::vector<char>>& map, const std::pair<int,int>& loc, const int& distance)
{
	std::vector<std::pair<int,int>> output;
	
	const int x_base = loc.first;
	const int y_base = loc.second;
	
	//const int x_max = (int)map[0].size();
	//const int y_max = (int)map.size();
	
	for(int x_offset = -distance; x_offset <= distance; x_offset++)
	{		
		for(int y_offset = -(distance - std::abs(x_offset)); y_offset <= (distance - std::abs(x_offset)); y_offset++)
		{
			const int x = x_base + x_offset;
			const int y = y_base + y_offset;
			
			//if(x < 0 || x >= x_max || y < 0 || y >= y_max) continue;
			
			output.push_back({x,y});
		}
	}
	
	return output;
}


void Day20::printMap(const std::vector<std::vector<char>>& map)
{	
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			std::cout << map[y][x];
		}
		std::cout << "\n";
	}
}

/**
 * INPUT
 */
bool Day20::readInput(std::vector<std::vector<char>> * map)
{
	std::string line;
	
	std::ifstream infile_map("data/day20.txt");

	map->clear();

	while(getline(infile_map, line))
	{
		std::vector<char> v;
		
		for(int i = 0; i < (int)line.size(); i++)
		{
			v.push_back(line[i]);
		}
		
		map->push_back(v);
	}	
		
	return true;
}


/**
 * TESTS 
 */
bool Day20::saveAtLeastLimitPicosecondsCheats_test()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
		                                   {'#','.','.','.','#','.','.','.','#','.','.','.','.','.','#'},
		                                   {'#','.','#','.','#','.','#','.','#','.','#','#','#','.','#'},
		                                   {'#','S','#','.','.','.','#','.','#','.','#','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','.','#','.','#','.','#','#','#'},
		                                   {'#','#','#','#','#','#','#','.','#','.','#','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','.','#','.','#','#','#','.','#'},
		                                   {'#','#','#','.','.','E','#','.','.','.','#','.','.','.','#'},
		                                   {'#','#','#','.','#','#','#','#','#','#','#','.','#','#','#'},
		                                   {'#','.','.','.','#','#','#','.','.','.','#','.','.','.','#'},
		                                   {'#','.','#','#','#','#','#','.','#','.','#','#','#','.','#'},
		                                   {'#','.','#','.','.','.','#','.','#','.','#','.','.','.','#'},
		                                   {'#','.','#','.','#','.','#','.','#','.','#','.','#','#','#'},
		                                   {'#','.','.','.','#','.','.','.','#','.','.','.','#','#','#'},
		                                   {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'} };
	
	std::cout << "\n";
	
	int result = saveAtLeastLimitPicosecondsCheats(map, 2, 0);
	
	if(result == 44)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day20::saveAtLeastLimitPicosecondsCheats_test2()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
		                                   {'#','.','.','.','#','.','.','.','#','.','.','.','.','.','#'},
		                                   {'#','.','#','.','#','.','#','.','#','.','#','#','#','.','#'},
		                                   {'#','S','#','.','.','.','#','.','#','.','#','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','.','#','.','#','.','#','#','#'},
		                                   {'#','#','#','#','#','#','#','.','#','.','#','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','.','#','.','#','#','#','.','#'},
		                                   {'#','#','#','.','.','E','#','.','.','.','#','.','.','.','#'},
		                                   {'#','#','#','.','#','#','#','#','#','#','#','.','#','#','#'},
		                                   {'#','.','.','.','#','#','#','.','.','.','#','.','.','.','#'},
		                                   {'#','.','#','#','#','#','#','.','#','.','#','#','#','.','#'},
		                                   {'#','.','#','.','.','.','#','.','#','.','#','.','.','.','#'},
		                                   {'#','.','#','.','#','.','#','.','#','.','#','.','#','#','#'},
		                                   {'#','.','.','.','#','.','.','.','#','.','.','.','#','#','#'},
		                                   {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'} };
	
	std::cout << "\n";
	
	int result = saveAtLeastLimitPicosecondsCheats(map, 20, 50);
	
	if(result == 285)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
