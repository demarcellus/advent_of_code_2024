#include "day8.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <cctype>

#include <algorithm>

bool Day8::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<char>> map;
	
	readInput(&map);
	
	// print
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[";
		for(size_t j = 0; j < 16; j++)
		{
			std::cout << map[i][j];
		}
		std::cout << "] ...\n";
	}
	std::cout << "...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = uniqueAntinodeLocations(map);
	std::cout << ">>> ANTINODE_LOCATIONS: [" << result << "]\n";
	int result2 = uniqueAntinodeLocations2(map);
	std::cout << ">>> ANTINODE_LOCATIONS 2: [" << result2 << "]\n";	
		
	return true;
}

bool Day8::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = uniqueAntinodeLocations_test();
	if(ok) ok = uniqueAntinodeLocations2_test();
	
	return ok;
}

/**
 * CODE
 */
int Day8::uniqueAntinodeLocations(std::vector<std::vector<char>> map)
{
	// antinode list
	std::vector<std::vector<int>> antinode_list;
	
	// global nice-to-haves
	int x_max = map[0].size();
	int y_max = map.size();
	
	// find all frequencies
	std::vector<char> frequencies;
	
	for(auto row : map)
	{
		for(auto pos : row)
		{
			if(std::isalnum(pos))
			{
				if(std::find(frequencies.begin(), frequencies.end(), pos) == frequencies.end())
				{
					frequencies.push_back(pos);
				}
			}
		}
	}
	
	std::cout << "[" << frequencies.size() << "] frequencies found\n";
	
	// for all frequencies
	for(auto frequency : frequencies)
	{
		std::cout << "frequency [" << frequency << "] --> ";
		
		std::vector<std::vector<int>> positions;
		
		// find antenna positions
		for(int x = 0; x < (int)map[0].size(); x++)
		{
			for(int y = 0; y < (int)map.size(); y++)
			{
				if(map[y][x] == frequency)
				{
					positions.push_back(std::vector<int> {x, y});
				}
			}
		}
		
		std::cout << "antenna positions found --> ";
		
		// make "empy" map (for debugging)
		//std::vector<std::vector<char>> map_antinodes(map.size(), std::vector<char>(map[0].size(), '.'));
				
		// for all positions, make pairs and determine antinodes
		for(int i = 0; i < (int)positions.size(); i++)
		{
			for(int j = 0; j < (int)positions.size(); j++)
			{
				if(i == j) continue;	// exclude self
				
				
				int antenna_distance_x = positions[j][0] - positions[i][0];
				int antenna_distance_y = positions[j][1] - positions[i][1];
				
				int antinode_1_x = positions[i][0] + 2 * antenna_distance_x;
				int antinode_1_y = positions[i][1] + 2 * antenna_distance_y;
				int antinode_2_x = positions[i][0] - antenna_distance_x;
				int antinode_2_y = positions[i][1] - antenna_distance_y;
											
				if(antinode_1_x >= 0 && antinode_1_x < x_max && antinode_1_y >= 0 && antinode_1_y < y_max)
				{
					//map_antinodes[antinode_1_y][antinode_1_x] = '#';
					
					std::vector<int> loc = {antinode_1_x, antinode_1_y};
					if(std::find(antinode_list.begin(), antinode_list.end(), loc) == antinode_list.end())
					{
						antinode_list.push_back(loc);
					}
				}
				if(antinode_2_x >= 0 && antinode_2_x < x_max && antinode_2_y >= 0 && antinode_2_y < y_max)
				{
					//map_antinodes[antinode_2_y][antinode_2_x] = '#';
					
					std::vector<int> loc = {antinode_2_x, antinode_2_y};
					if(std::find(antinode_list.begin(), antinode_list.end(), loc) == antinode_list.end())
					{
						antinode_list.push_back(loc);
					}
				}
			}
		}
		
		std::cout << "done\n";
		
		//printMap(&map_antinodes);
		//std::cout << "\n";
	}
	
	//std::cout << "antinode location list:\n";
	//for(auto loc : antinode_list)
	//{
	//	std::cout << "[" << loc[0] << "," << loc[1] << "]\n";
	//}
	//std::cout << "count [" << antinode_list.size() << "]\n";
	
	return antinode_list.size();
}

int Day8::uniqueAntinodeLocations2(std::vector<std::vector<char>> map)
{
	// antinode list
	std::vector<std::vector<int>> antinode_list;
	
	// global nice-to-haves
	int x_max = map[0].size();
	int y_max = map.size();
	
	// find all frequencies
	std::vector<char> frequencies;
	
	for(auto row : map)
	{
		for(auto pos : row)
		{
			if(std::isalnum(pos))
			{
				if(std::find(frequencies.begin(), frequencies.end(), pos) == frequencies.end())
				{
					frequencies.push_back(pos);
				}
			}
		}
	}
	
	std::cout << "[" << frequencies.size() << "] frequencies found\n";
	
	// for all frequencies
	for(auto frequency : frequencies)
	{
		std::cout << "frequency [" << frequency << "] --> ";
		
		std::vector<std::vector<int>> positions;
		
		// find antenna positions
		for(int x = 0; x < (int)map[0].size(); x++)
		{
			for(int y = 0; y < (int)map.size(); y++)
			{
				if(map[y][x] == frequency)
				{
					positions.push_back(std::vector<int> {x, y});
				}
			}
		}
		
		std::cout << "antenna positions found --> ";
		
		// make "empy" map (for debugging)
		//std::vector<std::vector<char>> map_antinodes(map.size(), std::vector<char>(map[0].size(), '.'));
				
		// for all positions, make pairs and determine antinodes
		for(int i = 0; i < (int)positions.size(); i++)
		{
			for(int j = 0; j < (int)positions.size(); j++)
			{
				if(i == j) continue;	// exclude self
				
				int antenna_distance_x = positions[j][0] - positions[i][0];
				int antenna_distance_y = positions[j][1] - positions[i][1];
						
				int p;
				
				// dir A
				p = 1;
				while(true)
				{
					int antinode_x = positions[i][0] + p * antenna_distance_x;
					int antinode_y = positions[i][1] + p * antenna_distance_y;
					
					if(antinode_x < 0 || antinode_x >= x_max || antinode_y < 0 || antinode_y >= y_max) break;
					
					//map_antinodes[antinode_y][antinode_x] = '#';
					
					std::vector<int> loc = {antinode_x, antinode_y};
					if(std::find(antinode_list.begin(), antinode_list.end(), loc) == antinode_list.end())
					{
						antinode_list.push_back(loc);
					}
					
					p++;
				}
				
				// dir B
				p = 0;
				while(true)
				{
					int antinode_x = positions[i][0] - p * antenna_distance_x;
					int antinode_y = positions[i][1] - p * antenna_distance_y;
					
					if(antinode_x < 0 || antinode_x >= x_max || antinode_y < 0 || antinode_y >= y_max) break;
					
					//map_antinodes[antinode_y][antinode_x] = '#';
					
					std::vector<int> loc = {antinode_x, antinode_y};
					if(std::find(antinode_list.begin(), antinode_list.end(), loc) == antinode_list.end())
					{
						antinode_list.push_back(loc);
					}
					
					p++;
				}
			}
		}
		
		std::cout << "done\n";
		
		//printMap(&map_antinodes);
		//std::cout << "\n";
	}
	
	//std::cout << "antinode location list:\n";
	//for(auto loc : antinode_list)
	//{
	//	std::cout << "[" << loc[0] << "," << loc[1] << "]\n";
	//}
	//std::cout << "count [" << antinode_list.size() << "]\n";
	
	return antinode_list.size();
}

void Day8::printMap(std::vector<std::vector<char>> * map)
{
	for(int y = 0; y < (int)map->size(); y++)
	{
		for(int x = 0; x < (int)map->at(y).size(); x++)
		{
			std::cout << map->at(y).at(x);
		}
		std::cout << "\n";
	}
}

/**
 * INPUT
 */
bool Day8::readInput(std::vector<std::vector<char>> * map)
{
	std::string line;
	
	// load rules
	std::ifstream infile("data/day8.txt");

	map->clear();

	while(getline(infile, line))
	{
		std::vector<char> w;
		
		for(size_t i = 0; i < line.size(); i++)
		{
			w.push_back(line[i]);
		}
		
		map->push_back(w);
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day8::uniqueAntinodeLocations_test()
{
	std::vector<std::vector<char>> map = { {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','0','.','.','.'},
                                           {'.','.','.','.','.','0','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','0','.','.','.','.'},
                                           {'.','.','.','.','0','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','A','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','A','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','A','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'} };
	
	std::cout << "\n";
	
	long result = uniqueAntinodeLocations(map);
	
	if(result == 14)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day8::uniqueAntinodeLocations2_test()
{
	std::vector<std::vector<char>> map = { {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','0','.','.','.'},
                                           {'.','.','.','.','.','0','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','0','.','.','.','.'},
                                           {'.','.','.','.','0','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','A','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','A','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','A','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.','.','.'} };
	
	std::cout << "\n";
	
	long result = uniqueAntinodeLocations2(map);
	
	if(result == 34)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
