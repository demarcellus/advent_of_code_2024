#include "day18.hpp"
		
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <queue>
#include <unordered_set>

#include <algorithm>

#include <climits>

struct Comparator
{
	bool operator()(const std::pair<std::pair<unsigned int, unsigned int>, int>& item_1, const std::pair<std::pair<unsigned int, unsigned int>, int>& item_2) const 
	{
		return item_1.second > item_2.second;   // second = movement cost up and including point
	}
};

bool Day18::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::pair<int,int>> byte_positions;
	
	readInput(&byte_positions);
	
	// print
	std::cout << "BYTE POSITIONS:\n";
	for(int i = 0; i < 10; i++)
	{
		std::cout << "[" << byte_positions[i].first << "," << byte_positions[i].second << "]\n";
	}
	std::cout << "...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = minimumStepsToExit(71, 71, byte_positions, 1024);
	std::cout << ">>> SHORTEST PATH: [" << result << "]\n";
	
	std::pair<int,int> result2 = firstBlockingByte(71, 71, byte_positions);
	std::cout << ">>> FIRST BLOCKER COORDINATES: [" << result2.first << "," << result2.second << "]\n";
					
	return true;
}

bool Day18::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = minimumStepsToExit_test();
	if(ok) ok = firstBlockingByte_test();

	return ok;
}

/**
 * CODE
 */
int Day18::minimumStepsToExit(const unsigned int size_x, const unsigned int size_y, const std::vector<std::pair<int, int>>& byte_positions, const unsigned int bytes_fallen)
{
	std::unordered_set<std::pair<int, int>, Hasher> path;
	
	std::cout << "DROP BYTES...\n";	
	auto map = dropBytes(size_x, size_y, byte_positions, bytes_fallen);
	
	std::cout << "MAP:\n";
	printMap(map, path);
	
	int best_path_score = findBestPathScore(map);
	
	
	//std::vector<std::vector<int>> best_scores_map = findBestScores(map, best_path_score);
	//path = walkBestPath(map, best_scores_map);
	//std::cout << "SOLUTION:\n";
	//printMap(map, path);
		
		
	return best_path_score;
}

std::pair<int,int> Day18::firstBlockingByte(const unsigned int size_x, const unsigned int size_y, const std::vector<std::pair<int,int>>& byte_positions)
{
	unsigned int first = 0;
	unsigned int last  = (unsigned int) byte_positions.size() - 1;
	
	unsigned int prev_check = 0;
	
	int i = 0;
	while(true)
	{
		if(i++ == 25) break;
		
		unsigned int check = first + (last - first)/2;
		if(check == prev_check)
		{
			//std::cout << "done [" << byte_positions[check].first << "," << byte_positions[check].second << "]\n";
			return byte_positions[check];
		}
		prev_check = check;
		
		//std::cout << "checking [" << check << "] --> ";
	
		auto map = dropBytes(size_x, size_y, byte_positions, check);
				
		int score = findBestPathScore(map);
		
		//std::cout << "score [" << score << "]\n";
		
		if(score == -1)
		{
			// first byte that cuts off is below this check
			if(last > check) last = check;
		}
		else
		{
			// first byte that cuts off is above this check
			if(first < check) first = check;
		}		
	}
	
	return std::pair<int,int> {-1, -1};
}

std::vector<std::vector<char>> Day18::dropBytes(const unsigned int size_x, const unsigned int size_y, const std::vector<std::pair<int, int>>& byte_positions, const unsigned int bytes_fallen)
{
	std::vector<std::vector<char>> map(size_y, std::vector<char>(size_x, '.'));
		
	// check limits
	unsigned int end = (unsigned int)byte_positions.size();
	if(bytes_fallen < end) end = bytes_fallen;
	
	for(unsigned int i = 0; i < bytes_fallen; i++)
	{
		unsigned int x = byte_positions[i].first;
		unsigned int y = byte_positions[i].second;
		map[y][x] = '#';
	}	
	
	return map;
}

int Day18::findBestPathScore(const std::vector<std::vector<char>>& map)
{
	const std::pair<int, int> start = {0, 0};
	const std::pair<int, int> end   = {(int)map[0].size()-1, (int)map.size()-1};

	const int x_min = 0;
	const int x_max = (unsigned int)map[0].size();
	const int y_min = 0;
	const int y_max = (unsigned int)map.size();
	
	// use a priority queue to find the shortest path
	// this will sort itself so we will solve for the most
	// likely path first
	std::priority_queue<std::pair<std::pair<int, int>, int>, std::vector<std::pair<std::pair<int, int>, int>>, Comparator> positions_score_queue;
	std::unordered_set<std::pair<int, int>, Hasher> positions_visited;
	
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
		if(current_position == end)
		{
			return current_score;
		}
		
		// move to positions around
		const std::vector<std::pair<int,int>> dirs = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		for(auto dir : dirs)
		{
			std::pair<int,int> next_position = { current_position.first + dir.first, current_position.second + dir.second };
			
			if(next_position.first < x_min || next_position.first >= x_max || next_position.second < y_min || next_position.second >= y_max)
			{
				// outside of map
				continue;
			}
			else if(map[next_position.second][next_position.first] == '#')
			{
				// we've hit a wall
				continue;
			}
			else
			{
				// we're in an empty space, save next point to check later
				positions_score_queue.push({next_position, current_score + 1});
			}
		}		
	}
	
	return -1;
}

std::vector<std::vector<int>> Day18::findBestScores(const std::vector<std::vector<char>>& map, const int best_path_score)
{
	const std::pair<int, int> end   = {(int)map[0].size()-1, (int)map.size()-1};
		
	const int x_min = 0;
	const int x_max = (unsigned int)map[0].size();
	const int y_min = 0;
	const int y_max = (unsigned int)map.size();
		
	// now we go backwards
	std::priority_queue<std::pair<std::pair<int, int>, int>, std::vector<std::pair<std::pair<int, int>, int>>, Comparator> positions_score_queue;
	std::unordered_set<std::pair<int, int>, Hasher> positions_visited;
	
	// output
	std::vector<std::vector<int>> best_score_map(map.size(), std::vector<int>(map[0].size(), -1));
	
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
		best_score_map[current_position.second][current_position.first] = current_score;
		
		// move to positions around
		const std::vector<std::pair<int,int>> dirs = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		for(auto dir : dirs)
		{
			std::pair<int,int> next_position = { current_position.first + dir.first, current_position.second + dir.second };
			
			if(next_position.first < x_min || next_position.first >= x_max || next_position.second < y_min || next_position.second >= y_max)
			{
				// outside of map
				continue;
			}
			else if(map[next_position.second][next_position.first] == '#')
			{
				// we've hit a wall
				continue;
			}
			else
			{
				// we're in an empty space, save next point to check later
				positions_score_queue.push({next_position, current_score + 1});
			}
		}			
	}
	
	return best_score_map;
}

std::unordered_set<std::pair<int, int>, Hasher> Day18::walkBestPath(const std::vector<std::vector<char>>& map, std::vector<std::vector<int>>& best_score_map)
{
	const std::pair<int, int> start = {0, 0};
	const std::pair<int, int> end   = {(int)map[0].size()-1, (int)map.size()-1};
		
	const int x_min = 0;
	const int x_max = (unsigned int)map[0].size();
	const int y_min = 0;
	const int y_max = (unsigned int)map.size();
		
	// forwards following the best path
	std::priority_queue<std::pair<std::pair<int, int>, int>, std::vector<std::pair<std::pair<int, int>, int>>, Comparator> positions_score_queue;
	std::unordered_set<std::pair<int, int>, Hasher> positions_visited;
		
	positions_score_queue.push({start, best_score_map[start.second][start.first]});
	
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
		if(current_position == end)
		{
			return positions_visited;   // one path
			//continue; 				// all path positions
		}
		
		// move to positions around
		const std::vector<std::pair<int,int>> dirs = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		for(auto dir : dirs)
		{
			std::pair<int,int> next_position = { current_position.first + dir.first, current_position.second + dir.second };
			
			if(next_position.first < x_min || next_position.first >= x_max || next_position.second < y_min || next_position.second >= y_max)
			{
				// outside of map
				continue;
			}
			else if(map[next_position.second][next_position.first] == '#')
			{
				// we've hit a wall
				continue;
			}
			else
			{
				int best_score = best_score_map[next_position.second][next_position.first];
				
				if(best_score == current_score - 1)				// check score - only best path
				{
					positions_score_queue.push({next_position, current_score - 1});								// save next point to check later
				}
			}
		}			
	}
		
	return positions_visited;
}


void Day18::printMap(const std::vector<std::vector<char>>& map, const std::unordered_set<std::pair<int, int>, Hasher>& list)
{
	std::vector<std::vector<char>> map_2 = map;
	
	for(auto& p : list)
	{
		map_2[p.second][p.first] = 'O';
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
bool Day18::readInput(std::vector<std::pair<int, int>> * byte_positions)
{
	std::string line;
	
	std::ifstream infile_map("data/day18.txt");

	byte_positions->clear();
	
	while(getline(infile_map, line))
	{
		int x,y;
		
		if(sscanf(line.c_str(),"%d,%d",&x,&y) != 2)
		{
			std::cout << "READ ERROR\n";
			return false;
		}

		byte_positions->push_back(std::pair<int,int> {x,y});
	}	
	
	return true;
}


/**
 * TESTS 
 */
bool Day18::minimumStepsToExit_test()
{
	std::vector<std::pair<int,int>> byte_positions = { {5,4},
									  	               {4,2},
									 	               {4,5},
										               {3,0},
										               {2,1},
										               {6,3},
										               {2,4},
										               {1,5},
										               {0,6},
										               {3,3},
										               {2,6},
										               {5,1},
										               {1,2},
										               {5,5},
										               {2,5},
										               {6,5},
										               {1,4},
										               {0,4},
										               {6,4},
										               {1,1},
										               {6,1},
										               {1,0},
										               {0,5},
										               {1,6},
										               {2,0} };
	
	std::cout << "\n";
	
	int result = minimumStepsToExit(7,7,byte_positions,12);
	
	if(result == 22)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day18::firstBlockingByte_test()
{
	std::vector<std::pair<int,int>> byte_positions = { {5,4},
									  	               {4,2},
									 	               {4,5},
										               {3,0},
										               {2,1},
										               {6,3},
										               {2,4},
										               {1,5},
										               {0,6},
										               {3,3},
										               {2,6},
										               {5,1},
										               {1,2},
										               {5,5},
										               {2,5},
										               {6,5},
										               {1,4},
										               {0,4},
										               {6,4},
										               {1,1},
										               {6,1},
										               {1,0},
										               {0,5},
										               {1,6},
										               {2,0} };
	
	std::cout << "\n";
	
	std::pair<int,int> result = firstBlockingByte(7,7,byte_positions);
	
	if(result == std::pair<int,int> {6,1})
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
