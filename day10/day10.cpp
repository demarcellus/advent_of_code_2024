#include "day10.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <algorithm>

bool Day10::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<int>> map;
	
	readInput(&map);
	
	// print
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[";
		for(size_t j = 0; j < 16; j++)
		{
			std::cout << map[i][j];
		}
		std::cout << " ... ]\n";
	}
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = trailheadScoreSum(map);
	std::cout << ">>> TRAILHEAD SCORE SUM: [" << result << "]\n";
	
	int result2 = trailheadRatingSum(map);
	std::cout << ">>> TRAILHEAD RATING SUM: [" << result2 << "]\n";
				
	return true;
}

bool Day10::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = trailheadScoreSum_test();
	if(ok) ok = trailheadRatingSum_test();
	
	return ok;
}

/**
 * CODE
 */
Node::Node(int _x, int _y, int _val)
{
	this->x = _x;
	this->y = _y;
	this->value = _val;
} 

Node * Day10::walkRecursive(std::vector<std::vector<int>>& map, int x, int y, int val_prev)
{
	std::string prefix;
	for(int i = 0; i < val_prev + 1; i++){ prefix += ">"; }
	
	//std::cout << prefix << "evaluating [" << x << "," << y << "]\n";
	
	// out of bounds
	if(x < 0 || x >= (int)map[0].size() || y < 0 || y >= (int)map.size())
	{
		//std::cout << prefix << "xy[" << x << "," << y << "] out of bounds\n";
		return nullptr;
	}
	
	int val = map[y][x];
	
	// check if one higher than previous step
	if(map[y][x] - val_prev != 1)
	{
		//std::cout << prefix << "xy[" << x << "," << y << "] height difference invalid [" << map[y][x] << " vs " << val_prev << "]\n";
		return nullptr;
	}
	
	// is this max height
	if(map[y][x] == 9)
	{
		//std::cout << prefix << "xy[" << x << "," << y << "] max height reached\n";
		Node * p = new Node(x, y, 9);
		//std::cout << prefix << "- making new node with value [" << val << "] children [" << p->children.size() <<"]\n";
		return p;
	}
	
	// check all directions
	std::vector<std::vector<int>> dirs = { {0,-1}, {1,0}, {0,1}, {-1,0} };
	std::vector<Node *> p_list;
	for(int i = 0 ; i < (int)dirs.size(); i++)
	{
		//std::cout << prefix << "- [" << i << "] trying dir [" << dirs[i][0] << "," << dirs[i][1] << "]\n";
		
		int x_next = x + dirs[i][0];
		int y_next = y + dirs[i][1];
		
		Node * result = walkRecursive(map, x_next, y_next, val);
		
		if(result == nullptr)
		{
			//std::cout << prefix << "- nothing found\n";
		}
		else
		{
			//std::cout << prefix << "- valid path found\n";
			p_list.push_back(result);
		}
	}
	
	// output
	if(!p_list.empty())
	{
		Node * p = new Node(x, y, val);
		p->children = p_list;
		
		//std::cout << prefix << "- making new node with value [" << val << "] children [" << p->children.size() <<"]\n";
		
		return p;
	}
	
	return nullptr;
}

int Day10::trailheadScoreSum(std::vector<std::vector<int>> map)
{
	std::vector<Node*> p_list;
	
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			// not interested in non-zero starting points
			if(map[y][x] != 0) continue;
			
			Node* p = walkRecursive(map, x, y, -1);
			
			if(p != nullptr)
			{
				p_list.push_back(p);
			}
			
		}
	}
		
	/*
	 * TRAILHEAD SCORE
	 */
	int sum = 0;
	 
	for(auto p : p_list)
	{
		std::vector<std::vector<int>> coordinates;
		countTrailheadScore(p, coordinates);
		
		int trailhead_score = coordinates.size();		
		sum += trailhead_score;
	}
	
	return sum;
}

int Day10::trailheadRatingSum(std::vector<std::vector<int>> map)
{
	std::vector<Node*> p_list;
	
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			// not interested in non-zero starting points
			if(map[y][x] != 0) continue;
			
			//std::cout << "START [" << x << "," << y <<"]\n";
			
			Node* p = walkRecursive(map, x, y, -1);
			
			if(p != nullptr)
			{
				p_list.push_back(p);
			}
			
		}
	}
		
	/*
	 * TRAILHEAD SCORE
	 */
	int sum = 0;
	
	for(auto p : p_list)
	{
		int rating = 0;
		countTrailheadRating(p, rating);
	
		sum += rating;
	}
	
	return sum;
}

void Day10::countTrailheadScore(Node * p, std::vector<std::vector<int>>& coordinates)
{
	if(p->value == 9)
	{
		std::vector<int> c = {p->x, p->y};
		if(std::find(coordinates.begin(), coordinates.end(), c) == coordinates.end())
		{
			coordinates.push_back(c);
		}
	}
	
	for(auto child : p->children)
	{
		countTrailheadScore(child, coordinates);
	}
}

void Day10::countTrailheadRating(Node * p, int& count)
{
	if(p->value == 9)
	{
		count++;
	}
	
	for(auto child : p->children)
	{
		countTrailheadRating(child, count);
	}
}

void Day10::placeOnMap(std::vector<std::vector<char>>& map, Node * p, int depth, int until)
{
	if(depth == until) return;
	
	map[p->y][p->x] = (p->value + '0');
	
	for(auto child : p->children)
	{
		placeOnMap(map, child, depth + 1, until);
	}
}

void Day10::printMap(std::vector<std::vector<char>>& map)
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
bool Day10::readInput(std::vector<std::vector<int>> * map)
{
	std::string line;
	
	// load rules
	std::ifstream infile("data/day10.txt");

	map->clear();

	while(getline(infile, line))
	{
		std::vector<int> w;
		
		for(int i = 0; i < (int)line.size(); i++)
		{
			int val = (line[i] - '0');
			w.push_back(val);
		}
		
		map->push_back(w);
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day10::trailheadScoreSum_test()
{
	std::vector<std::vector<int>> map = { { 8,9,0,1,0,1,2,3 },
		                                  { 7,8,1,2,1,8,7,4 },
		                                  { 8,7,4,3,0,9,6,5 },
		                                  { 9,6,5,4,9,8,7,4 },
		                                  { 4,5,6,7,8,9,0,3 },
		                                  { 3,2,0,1,9,0,1,2 },
		                                  { 0,1,3,2,9,8,0,1 },
		                                  { 1,0,4,5,6,7,3,2 } };
	
	std::cout << "\n";
	
	long result = trailheadScoreSum(map);
	
	if(result == 36)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day10::trailheadRatingSum_test()
{
	std::vector<std::vector<int>> map = { { 8,9,0,1,0,1,2,3 },
		                                  { 7,8,1,2,1,8,7,4 },
		                                  { 8,7,4,3,0,9,6,5 },
		                                  { 9,6,5,4,9,8,7,4 },
		                                  { 4,5,6,7,8,9,0,3 },
		                                  { 3,2,0,1,9,0,1,2 },
		                                  { 0,1,3,2,9,8,0,1 },
		                                  { 1,0,4,5,6,7,3,2 } };
	
	std::cout << "\n";
	
	long result = trailheadRatingSum(map);
	
	if(result == 81)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
