#include "day16.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <queue>
#include <unordered_set>


struct Comparator
{
	bool operator()(const std::pair<Position, int>& item_1, const std::pair<Position, int>& item_2) const 
	{
		return item_1.second > item_2.second;   // second = movement cost up and including point
	}
};

struct Hasher
{
	size_t operator()(const Position& pos) const
	{
		return (1000000 * pos.x) + (1000 * pos.y) + pos.dir;
	}
};

bool Day16::run()
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
	
	long result = solveMaze(map);
	std::cout << ">>> GPS COORDINATES SUM: [" << result << "]\n";
					
	return true;
}

bool Day16::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = solveMaze_test();
	if(ok) ok = solveMaze_test2();
	
	return ok;
}

/**
 * CODE
 */
long Day16::solveMaze(std::vector<std::vector<char>> map)
{
	Position start = findStart(&map);
	Position end = findEnd(&map);
	
	//std::cout << "Start at [" << start.x << "," << start.y << "]\n";
	//std::cout << "End at [" << end.x << "," << end.y << "]\n";
	
	// use a priority queue to find the shortest path
	// this will sort itself so we will solve for the most
	// likely path first
	std::priority_queue<std::pair<Position, int>, std::vector<std::pair<Position, int>>, Comparator> positions_score_queue;
	std::unordered_set<Position, Hasher> positions_visited;
	
	positions_score_queue.push({start, 0});
	
	// continue until goal reached or queue empty
	while(!positions_score_queue.empty())
	{
		// save topmost entry
		const auto [current_position, current_score] = positions_score_queue.top();
		positions_score_queue.pop();
	
		//std::cout << "Evaluating [" << current_position.x << "," << current_position.y << "][" << current_score << "]\n";
		
		// already visited? if not, save
		if(positions_visited.count(current_position))
		{
			//std::cout << "Already visited\n";
			continue;
		}
		positions_visited.insert(current_position);
		
		// are we at the end?
		if(current_position.equals(end))
		{
			//std::cout << "Done! [" << current_score << "]\n";
			return current_score;
		}
		
		// move to positions around
		std::vector<Position> dirs = { {0, -1, 0}, {1, 0, 1}, {0, 1, 2}, {-1, 0, 3} };
		for(auto dir : dirs)
		{
			Position next_position = { current_position.x + dir.x, current_position.y + dir.y, dir.dir };
			
			//std::cout << "Trying [" << next_position.x << "," << next_position.y << "," << next_position.dir << "]\n";
			
			if(map.at(next_position.y).at(next_position.x) == '#')
			{
				// we've hit a wall
				//std::cout << "Hit a wall\n";
				continue;
			}
			else
			{
				// we're in an empty space
				if(current_position.dir == next_position.dir)
				{
					//std::cout << "Straight\n";
					positions_score_queue.push({next_position, current_score + 1});		// save next point to check later
				}
				else
				{
					//std::cout << "Corner\n";
					positions_score_queue.push({next_position, current_score + 1001});	// save next point to check later
				}
			}
		}
		
	}
	
	return -1;
}

Position Day16::findStart(std::vector<std::vector<char>> * map)
{
	for(int y = 0; y < (int)map->size(); y++)
	{
		for(int x = 0; x < (int)map->at(y).size(); x++)
		{
			if(map->at(y).at(x) == 'S')
			{		
				return {x, y, 1};
			}
		}
	}
	
	return {0,0,0};
}

Position Day16::findEnd(std::vector<std::vector<char>> * map)
{
	for(int y = 0; y < (int)map->size(); y++)
	{
		for(int x = 0; x < (int)map->at(y).size(); x++)
		{
			if(map->at(y).at(x) == 'E')
			{		
				return {x, y, 1};
			}
		}
	}
	
	return {0,0,0};
}

/**
 * INPUT
 */
bool Day16::readInput(std::vector<std::vector<char>> * map)
{
	std::string line;
	
	std::ifstream infile_map("data/day16.txt");

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
bool Day16::solveMaze_test()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
										   {'#','.','.','.','.','.','.','.','#','.','.','.','.','E','#'},
										   {'#','.','#','.','#','#','#','.','#','.','#','#','#','.','#'},
										   {'#','.','.','.','.','.','#','.','#','.','.','.','#','.','#'},
										   {'#','.','#','#','#','.','#','#','#','#','#','.','#','.','#'},
										   {'#','.','#','.','#','.','.','.','.','.','.','.','#','.','#'},
										   {'#','.','#','.','#','#','#','#','#','.','#','#','#','.','#'},
										   {'#','.','.','.','.','.','.','.','.','.','.','.','#','.','#'},
										   {'#','#','#','.','#','.','#','#','#','#','#','.','#','.','#'},
										   {'#','.','.','.','#','.','.','.','.','.','#','.','#','.','#'},
										   {'#','.','#','.','#','.','#','#','#','.','#','.','#','.','#'},
										   {'#','.','.','.','.','.','#','.','.','.','#','.','#','.','#'},
										   {'#','.','#','#','#','.','#','.','#','.','#','.','#','.','#'},
										   {'#','S','.','.','#','.','.','.','.','.','#','.','.','.','#'},
										   {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'} };
	
	std::cout << "\n";
	
	long result = solveMaze(map);
	
	if(result == 7036)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day16::solveMaze_test2()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
		                                   {'#','.','.','.','#','.','.','.','#','.','.','.','#','.','.','E','#'},
		                                   {'#','.','#','.','#','.','#','.','#','.','#','.','#','.','#','.','#'},
		                                   {'#','.','#','.','#','.','#','.','.','.','#','.','.','.','#','.','#'},
		                                   {'#','.','#','.','#','.','#','.','#','#','#','.','#','.','#','.','#'},
		                                   {'#','.','.','.','#','.','#','.','#','.','.','.','.','.','#','.','#'},
		                                   {'#','.','#','.','#','.','#','.','#','.','#','#','#','#','#','.','#'},
		                                   {'#','.','#','.','.','.','#','.','#','.','#','.','.','.','.','.','#'},
		                                   {'#','.','#','.','#','#','#','#','#','.','#','.','#','#','#','.','#'},
		                                   {'#','.','#','.','#','.','.','.','.','.','.','.','#','.','.','.','#'},
		                                   {'#','.','#','.','#','#','#','.','#','#','#','#','#','.','#','#','#'},
		                                   {'#','.','#','.','#','.','.','.','#','.','.','.','.','.','#','.','#'},
		                                   {'#','.','#','.','#','.','#','#','#','#','#','.','#','#','#','.','#'},
		                                   {'#','.','#','.','#','.','.','.','.','.','.','.','.','.','#','.','#'},
		                                   {'#','.','#','.','#','.','#','#','#','#','#','#','#','#','#','.','#'},
		                                   {'#','S','#','.','.','.','.','.','.','.','.','.','.','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'} };
	
	std::cout << "\n";
	
	long result = solveMaze(map);
	
	if(result == 11048)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
