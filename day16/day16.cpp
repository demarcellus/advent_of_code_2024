#include "day16.hpp"
		
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <queue>
#include <unordered_set>

#include <algorithm>

struct Comparator
{
	bool operator()(const std::pair<Position, int>& item_1, const std::pair<Position, int>& item_2) const 
	{
		return item_1.second > item_2.second;   // second = movement cost up and including point
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
	
	int result = solveMaze(map);
	std::cout << ">>> SHORTEST PATH: [" << result << "]\n";
					
	int result2 = countTilesAlongBestPaths(map);
	std::cout << ">>> TILES ALONG BEST PATHS: [" << result2 << "]\n";					
					
	return true;
}

bool Day16::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = solveMaze_test();
	if(ok) ok = solveMaze_test2();
	
	if(ok) ok = countTilesAlongBestPaths_test();
	if(ok) ok = countTilesAlongBestPaths_test2();
	
	return ok;
}

/**
 * CODE
 */
int Day16::solveMaze(const std::vector<std::vector<char>> map)
{
	auto [best_path_score, end_dir] = findBestPath(map);
	
	return best_path_score;
}

int Day16::countTilesAlongBestPaths(const std::vector<std::vector<char>> map)
{
	auto [best_path_score, end_dir] = findBestPath(map);
	
	auto best_score_map = findBestScores(map, best_path_score, end_dir);
	auto positions = walkBestPaths(map, best_score_map);
	
	return countUniquePositions(positions);
}

Position Day16::findStart(const std::vector<std::vector<char>>& map)
{
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			if(map[y][x] == 'S')
			{		
				return {x, y, 1};
			}
		}
	}
	
	return {0,0,0};
}

Position Day16::findEnd(const std::vector<std::vector<char>>& map)
{
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			if(map[y][x] == 'E')
			{		
				return {x, y, 1};
			}
		}
	}
	
	return {0,0,0};
}

std::pair<int,int> Day16::findBestPath(const std::vector<std::vector<char>>& map)
{
	Position start = findStart(map);
	Position end = findEnd(map);
	
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
		
		// already visited? if not, save
		if(positions_visited.count(current_position))
		{
			continue;
		}
		positions_visited.insert(current_position);
		
		// are we at the end?
		if(current_position.equals(end))
		{
			return {current_score, current_position.dir};
		}
		
		// move to positions around
		const std::vector<Position> dirs = { {0, -1, 0}, {1, 0, 1}, {0, 1, 2}, {-1, 0, 3} };
		for(auto dir : dirs)
		{
			Position next_position = { current_position.x + dir.x, current_position.y + dir.y, dir.dir };
			
			if(map.at(next_position.y).at(next_position.x) == '#')
			{
				// we've hit a wall
				continue;
			}
			else
			{
				// we're in an empty space
				if(next_position.moveStraight(current_position))
				{
					positions_score_queue.push({next_position, current_score + 1});		// save next point to check later
				}
				else if(next_position.moveLeftRight(current_position))
				{
					positions_score_queue.push({next_position, current_score + 1001});	// save next point to check later
				}
				else if(next_position.moveBackwards(current_position))
				{
					positions_score_queue.push({next_position, current_score + 2001});	// save next point to check later
				}
			}
		}		
	}
	
	return {-1,-1};
}

std::vector<std::vector<std::vector<int>>> Day16::findBestScores(const std::vector<std::vector<char>>& map, const int best_path_score, const int end_dir)
{
	Position end = findEnd(map);
	end.dir = end_dir;
		
	// now we go backwards
	std::priority_queue<std::pair<Position, int>, std::vector<std::pair<Position, int>>, Comparator> positions_score_queue;
	std::unordered_set<Position, Hasher> positions_visited;
	
	// output
	std::vector<std::vector<std::vector<int>>> best_score_map(map.size(), std::vector<std::vector<int>>(map[0].size(), {-1, -1, -1, -1}));
	
	positions_score_queue.push({end, 0});
	
	// continue until goal reached or queue empty
	while(!positions_score_queue.empty())
	{
		// save topmost entry
		const auto [current_position, current_score] = positions_score_queue.top();
		positions_score_queue.pop();
		
		// already visited? if not, save
		if(positions_visited.count(current_position))
		{
			continue;
		}
		positions_visited.insert(current_position);
		
		// save score
		best_score_map[current_position.y][current_position.x][current_position.dir] = current_score;
		
		// move to next point with current dir
		const std::vector<Position> dirs = { {0, -1, 0}, {1, 0, 1}, {0, 1, 2}, {-1, 0, 3} };
		const Position dir = dirs[current_position.dir];
		Position next_position = { current_position.x - dir.x, current_position.y - dir.y, current_position.dir };	// reverse because we start at the end
		
		if(map.at(next_position.y).at(next_position.x) == '#')
		{
			// we've hit a wall
			continue;
		}
		
		// move to directions around		
		for(int try_dir = 0; try_dir < 4; try_dir++)
		{
			next_position.dir = try_dir;
			
			if(next_position.moveStraight(current_position))
			{
				positions_score_queue.push({next_position, current_score + 1});		// save next point to check later
			}
			else if(next_position.moveLeftRight(current_position))
			{
				positions_score_queue.push({next_position, current_score + 1001});	// save next point to check later
			}
			else if(next_position.moveBackwards(current_position))
			{
				positions_score_queue.push({next_position, current_score + 2001});	// save next point to check later
			}
		}		
	}
	
	return best_score_map;
}

std::unordered_set<Position, Hasher> Day16::walkBestPaths(const std::vector<std::vector<char>>& map, std::vector<std::vector<std::vector<int>>>& best_score_map)
{
	Position start = findStart(map);
	Position end = findEnd(map);
		
	// forwards following the best path
	std::priority_queue<std::pair<Position, int>, std::vector<std::pair<Position, int>>, Comparator> positions_score_queue;
	std::unordered_set<Position, Hasher> positions_visited;
		
	positions_score_queue.push({start, best_score_map[start.y][start.x][start.dir]});
	
	// continue until goal reached or queue empty
	while(!positions_score_queue.empty())
	{
		// save topmost entry
		const auto [current_position, current_score] = positions_score_queue.top();
		positions_score_queue.pop();
		
		// already visited? if not, save
		if(positions_visited.count(current_position))
		{
			continue;
		}
		positions_visited.insert(current_position);
		
		// are we at the end?
		if(current_position.equals(end))
		{
			continue; // give other paths a chance
		}
		
		// move to positions around
		const std::vector<Position> dirs = { {0, -1, 0}, {1, 0, 1}, {0, 1, 2}, {-1, 0, 3} };
		for(auto dir : dirs)
		{
			Position next_position = { current_position.x + dir.x, current_position.y + dir.y, dir.dir };
			
			if(map.at(next_position.y).at(next_position.x) == '#')
			{
				// we've hit a wall
				continue;
			}
			else
			{
				int best_score = best_score_map[next_position.y][next_position.x][next_position.dir];
				
				if(next_position.moveStraight(current_position) && best_score == current_score - 1)				// check score - only best path
				{
					positions_score_queue.push({next_position, current_score - 1});								// save next point to check later
				}
				else if(next_position.moveLeftRight(current_position) && best_score == current_score - 1001)    // check score - only best path
				{
					positions_score_queue.push({next_position, current_score - 1001});							// save next point to check later
				}
				else if(next_position.moveBackwards(current_position) && best_score == current_score - 2001)    // check score - only best path
				{
					positions_score_queue.push({next_position, current_score - 2001});							// save next point to check later
				}
			}
		}		
	}
	
	// print map with positions
	//printMap(map, positions_visited);
	
	return positions_visited;
}

int Day16::countUniquePositions(const std::unordered_set<Position, Hasher>& list)
{
	std::vector<Position> poss;
	
	for(auto& p : list)
	{
		Position pos = p;
		pos.dir = 0;
		if(std::find(poss.begin(), poss.end(), pos) == poss.end())
		{
			poss.push_back(pos);
		}
	}
	
	return (int)poss.size();
}

void Day16::printMap(const std::vector<std::vector<char>>& map, const std::unordered_set<Position, Hasher>& list)
{
	std::vector<std::vector<char>> map_2 = map;
	
	for(auto& p : list)
	{
		map_2[p.y][p.x] = 'O';
	}
	
	for(int y = 0; y < (int)map_2.size(); y++)
	{
		for(int x = 0; x < (int)map_2[y].size(); x++)
		{
			std::cout << map_2[y][x];
		}
		std::cout << "\n";
	}
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

bool Day16::countTilesAlongBestPaths_test()
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
	
	long result = countTilesAlongBestPaths(map);
	
	if(result == 45)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day16::countTilesAlongBestPaths_test2()
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
	
	long result = countTilesAlongBestPaths(map);
	
	if(result == 64)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
