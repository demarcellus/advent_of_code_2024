#include "day14.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <algorithm>
#include <execution>

#include <climits>
#include <cstdio>


Bathroom::Bathroom(int _width, int _height)
{
	this->width = _width;
	this->height = _height;
}

Robot::Robot(int _x, int _y, int _vx, int _vy)
{
	this->x = _x;
	this->y = _y;
	this->vx = _vx;
	this->vy = _vy;
}

void Robot::move(Bathroom& bathroom)
{
	//std::cout << "ROBOT move from [" << this->x << "," << this->y << "] to ";
	
	this->x = (this->x + this->vx);
	if(this->x < 0) this->x += bathroom.width;
	if(this->x >= bathroom.width) this->x -= bathroom.width;
	
	this->y = (this->y + this->vy);
	if(this->y < 0) this->y += bathroom.height;
	if(this->y >= bathroom.height) this->y -= bathroom.height;;
	
	//std::cout << "[" << this->x << "," << this->y << "]\n";
}

bool Day14::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<Robot *> robots;
	
	readInput(&robots);
	
	// print
	for(int i = 0; i < 5; i++)
	{
		std::cout << "ROBOT START [" << robots[i]->x << "," << robots[i]->y << "] SPEED [" << robots[i]->vx << "," << robots[i]->vy << "]\n";
	}
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	long result = calculateSafetyFactor(robots, Bathroom(101, 103), 100);
	std::cout << ">>> SAFETY FACTOR: [" << result << "]\n";
	
	int result2 = findEasterEgg(robots, Bathroom(101,103), 100000);
	std::cout << ">>> EASTER EGG: [" << result2 << "]\n";
				
	return true;
}

bool Day14::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = calculateSafetyFactor_test();
	
	return ok;
}

/**
 * CODE
 */
long Day14::calculateSafetyFactor(std::vector<Robot *> robots, Bathroom bathroom, int iterations)
{	
	for(int i = 0; i < iterations; i++)
	{
		for(auto robot_p : robots)
		{
			robot_p->move(bathroom);
		}
	}
	
	std::vector<int> quadrants = calculateRobotsInQuadrants(robots, bathroom);
	
	return std::reduce(quadrants.begin(), quadrants.end(), 1, std::multiplies<long>());
}

int Day14::findEasterEgg(std::vector<Robot *> robots, Bathroom bathroom, int max_iterations)
{
	// wow... so the easter egg is a chrismas tree
	// so the robots are clustered or something?
	
	//std::vector<int> easter_iterations;
	int easter_iteration = -1;
	
	for(int i = 1; i < max_iterations; i++)
	{
		for(auto robot_p : robots)
		{
			robot_p->move(bathroom);
		}
		
		if(detectCluster(robots, bathroom))
		{
			//easter_iterations.push_back(i);
			
			std::cout << "ITERATION [" << i << "] --> CLUSTER FOUND\n";
			printBathroom(robots, bathroom);
			
			char in;
			do
			{
				std::cout << "Press [y] if Christmas Tree, [n] if not... \n";
				in = std::cin.get();
			} while (in != 'y' && in != 'n');
			
			if(in == 'y')
			{
				easter_iteration = i;
				break;
			}
		}
	}
	
	//for(auto easter_iteration : easter_iterations)
	//{
	//	std::cout << ">>> [" << easter_iteration << "]\n";
	//}
	
	//return easter_iterations[0];
	return easter_iteration;
}

bool Day14::detectCluster(std::vector<Robot *>& robots, Bathroom& bathroom)
{
	std::vector<std::vector<int>> map(bathroom.height, std::vector<int>(bathroom.width, 0));
	
	for(auto robot_p : robots)
	{
		map[robot_p->y][robot_p->x]++;
	}
	
	int reduce_factor = 5;
	
	std::vector<std::vector<int>> reduced;
	
	for(int y = 0; y < (int)map.size(); y += reduce_factor)
	{
		std::vector<int> v;
		
		for(int x = 0; x < (int)map[y].size(); x += reduce_factor)
		{
			int count = 0;
			
			for(int dy = 0; dy < 5; dy++)
			{
				for(int dx = 0; dx < 5; dx++)
				{
					if(y + dy >= (int)map.size() || x + dx >= (int)map[0].size()) continue;
					
					count += (map[y + dy][x + dx] > 0 ? 1 : 0);
				}
			}
			
			int limit = (reduce_factor * reduce_factor * 0.5);
			if(count >= limit)
			{
				//std::cout << "COUNT [" << count << "] > [" << limit << "]\n";
				return true;
			}
		}
	}
	
	
	
	return false;
	
}

std::vector<int> Day14::calculateRobotsInQuadrants(std::vector<Robot *>& robots, Bathroom& bathroom)
{
	std::vector<std::vector<int>> map(bathroom.height, std::vector<int>(bathroom.width, 0));
	
	for(auto robot_p : robots)
	{
		map[robot_p->y][robot_p->x]++;
	}
	
	std::vector<int> output;
	std::vector<std::vector<int>> quadrants = { {0,                       (bathroom.width - 1) / 2, 0,                         (bathroom.height - 1) / 2},
                                                {(bathroom.width + 1) /2, bathroom.width,           0,                         (bathroom.height - 1) / 2},
                                                {0,                       (bathroom.width - 1) / 2, (bathroom.height + 1) / 2, bathroom.height},
                                                {(bathroom.width + 1) /2, bathroom.width,           (bathroom.height + 1) / 2, bathroom.height} };
	
	for(auto quadrant : quadrants)
	{
		int count = 0;
				
		for(int y = quadrant[2]; y < quadrant[3]; y++)
		{
			for(int x = quadrant[0]; x < quadrant[1]; x++)
			{
				count += map[y][x];
			}
		}
		
		output.push_back(count);	
	}
	
	return output;
}

void Day14::printBathroom(std::vector<Robot *>& robots, Bathroom& bathroom)
{
	std::vector<std::vector<int>> map(bathroom.height, std::vector<int>(bathroom.width, 0));
	
	for(auto robot_p : robots)
	{
		map[robot_p->y][robot_p->x]++;
	}
	
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			int val = map[y][x];
			
			if(val == 0) std::cout << ".";
			else         std::cout << map[y][x];
		}
		std::cout << "\n";
	}
}


/**
 * INPUT
 */
bool Day14::readInput(std::vector<Robot *> * robots)
{
	std::string line;
	
	std::ifstream infile("data/day14.txt");

	robots->clear();
	
	while(getline(infile, line))
	{
		int x, y, vx, vy;
		
		if(std::sscanf(line.c_str(),"p=%d,%d v=%d,%d",&x,&y,&vx,&vy) != 4){ std::cout << "READ ERROR"; return false; }
		
		robots->push_back(new Robot(x,y,vx,vy));
	}	
	
	return true;
}


/**
 * TESTS 
 */
bool Day14::calculateSafetyFactor_test()
{
	std::vector<Robot *> robots = { new Robot(0, 4, 3, -3),
		                            new Robot(6, 3, -1, -3),
		                            new Robot(10, 3, -1, 2),
		                            new Robot(2, 0, 2, -1),
		                            new Robot(0, 0, 1, 3),
		                            new Robot(3, 0, -2, -2),
		                            new Robot(7, 6, -1, -3),
		                            new Robot(3, 0, -1, -2),
		                            new Robot(9, 3, 2, 3),
		                            new Robot(7, 3, -1, 2),
		                            new Robot(2, 4, 2, -3),
		                            new Robot(9, 5, -3, -3) };
	
	std::cout << "\n";
	
	long result = calculateSafetyFactor(robots, Bathroom(11, 7), 100);
	
	if(result == 12)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
