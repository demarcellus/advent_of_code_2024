#include "day15.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <algorithm>
#include <execution>


Object::Object(Type _type, int _x, int _y)
{
	this->type = _type;
	this->x = _x;
	this->y = _y;
}

bool Object::move(std::vector<Object *>& objects, char direction, int depth)
{
	int  x_next;
	int  y_next;
	
	//std::string header;
	//for(int i = 0; i < depth; i++)
	//{
	//	header.append(">");
	//}
	
	if(this->type == Object::Type::Wall)
	{
		//std::cout << header << "I'm a wall - no move\n";
		return false;
	}
	
	switch(direction)
	{
		case '^':
			x_next = this->x;
			y_next = this->y - 1;
			break;
		case '>':
			x_next = this->x + 1;
			y_next = this->y;
			break;
		case 'v':
			x_next = this->x;
			y_next = this->y + 1;
			break;
		case '<':
			x_next = this->x - 1;
			y_next = this->y;
			break;
		default:
			std::cout << "MOVE ERROR";
			return false;
	}
	
	bool next_found = false;
	bool next_did_move = false;
	
	for(auto object : objects)
	{
		if(object->x == x_next && object->y == y_next)
		{
			//std::cout << header << "Object found at [" << x_next << "," << y_next << "]\n";
			
			next_found = true;
			next_did_move = object->move(objects, direction, depth + 1);
			break;
		}
	}
	
	//if(!next_found)
	//{
	//	std::cout << header << "No object found at [" << x_next << "," << y_next << "]\n";
	//}
	
	if(!next_found || next_did_move)
	{
		//std::cout << header << "No object found at [" << x_next << "," << y_next << "] or next did move\n";
		
		this->x = x_next;
		this->y = y_next;
		
		return true;
	}
	
	return false;
}

bool Day15::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<char>> map;
	std::vector<char> moves;
	
	readInput(&map, &moves);
	
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
	
	std::cout << "MOVES:\n";
	for(int i = 0; i < 16; i++)
	{
		std::cout << moves[i];
	}
	std::cout << " ...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	long result = sumBoxGpsCoordinates(map, moves);
	std::cout << ">>> GPS COORDINATES SUM: [" << result << "]\n";
					
	return true;
}

bool Day15::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = sumBoxGpsCoordinates_small_test();
	if(ok) ok = sumBoxGpsCoordinates_large_test();
	
	return ok;
}

/**
 * CODE
 */
long Day15::sumBoxGpsCoordinates(std::vector<std::vector<char>> map, std::vector<char> moves)
{
	std::vector<Object *> objects;
	Object robot = Object(Object::Type::Robot, -1, -1);
	
	// parse map to object array
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			char val = map[y][x];
			
			switch(val)
			{
				case '#': 
				
					objects.push_back(new Object(Object::Type::Wall, x, y));
					break;
				
				case 'O':
				
					objects.push_back(new Object(Object::Type::Box, x, y));
					break;
					
				case '@':
				
					robot.x = x;
					robot.y = y;
					break;
			}					
		}
	}
	
	// initial
	//std::cout << "MOVE [0]\n";
	//printObjects(map, objects, robot);	
	//std::cout << "\n";		
	
	// go move!
	//int i = 1;
	for(auto move : moves)
	{
		//std::cout << "[" << (i++) << "] MOVE [" << move << "]\n";
		
		robot.move(objects, move, 0);

		//printObjects(map, objects, robot);	
		//std::cout << "\n";		
	}
	
	
	int coordinate_sum = 0;
	for(auto object : objects)
	{
		if(object->type == Object::Type::Box)
		{
			int coordinate = (object->y * 100) + object->x;
		
			coordinate_sum += coordinate;
		}
	}
	
	//std::cout << "COORDINATE SUM [" << coordinate_sum << "]\n";
	
	return coordinate_sum;
}

void Day15::printObjects(std::vector<std::vector<char>>& map, std::vector<Object *>& objects, Object& robot)
{
	std::vector<std::vector<char>> m(map.size(), std::vector<char>(map[0].size(), '.'));
	
	for(auto object : objects)
	{
		char val = 'x';
		
		if(object->type == Object::Type::Wall) val = '#';
		else if(object->type == Object::Type::Box) val = 'O';
		
		m[object->y][object->x] = val;
	}
	
	m[robot.y][robot.x] = '@';
	
	for(int y = 0; y < (int)m.size(); y++)
	{
		for(int x = 0; x < (int)m[y].size(); x++)
		{
			std::cout << m[y][x];
		}
		std::cout << "\n";
	}

}

/**
 * INPUT
 */
bool Day15::readInput(std::vector<std::vector<char>> * map, std::vector<char> * moves)
{
	std::string line;
	
	std::ifstream infile_map("data/day15_map.txt");
	std::ifstream infile_moves("data/day15_moves.txt");

	map->clear();
	moves->clear();
	
	while(getline(infile_map, line))
	{
		std::vector<char> v;
		
		for(int i = 0; i < (int)line.size(); i++)
		{
			v.push_back(line[i]);
		}
		
		map->push_back(v);
	}	
	
	while(getline(infile_moves, line))
	{	
		for(int i = 0; i < (int)line.size(); i++)
		{
			moves->push_back(line[i]);
		}
	}	
	
	return true;
}


/**
 * TESTS 
 */
bool Day15::sumBoxGpsCoordinates_small_test()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#','#'},
		                                   {'#','.','.','O','.','O','.','#'},
		                                   {'#','#','@','.','O','.','.','#'},
		                                   {'#','.','.','.','O','.','.','#'},
		                                   {'#','.','#','.','O','.','.','#'},
		                                   {'#','.','.','.','O','.','.','#'},
		                                   {'#','.','.','.','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','#'} };
	
	std::vector<char> moves = {'<','^','^','>','>','>','v','v','<','v','>','>','v','<','<'};
	
	std::cout << "\n";
	
	long result = sumBoxGpsCoordinates(map, moves);
	
	if(result == 2028)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day15::sumBoxGpsCoordinates_large_test()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#','#','#','#'},
		                                   {'#','.','.','O','.','.','O','.','O','#'},
		                                   {'#','.','.','.','.','.','.','O','.','#'},
		                                   {'#','.','O','O','.','.','O','.','O','#'},
		                                   {'#','.','.','O','@','.','.','O','.','#'},
		                                   {'#','O','#','.','.','O','.','.','.','#'},
		                                   {'#','O','.','.','O','.','.','O','.','#'},
		                                   {'#','.','O','O','.','O','.','O','O','#'},
		                                   {'#','.','.','.','.','O','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#','#','#','#'} };
	
	std::vector<char> moves = { '<','v','v','>','^','<','v','^','>','v','>','^','v','v','^','v','>','v','<','>','v','^','v','<','v','<','^','v','v','<','<','<','^','>','<','<','>','<','>','>','v','<','v','v','v','<','>','^','v','^','>','^','<','<','<','>','<','<','v','<','<','<','v','^','v','v','^','v','>','^',
                                'v','v','v','<','<','^','>','^','v','^','^','>','<','<','>','>','>','<','>','^','<','<','>','<','^','v','v','^','^','<','>','v','v','v','<','>','>','<','^','^','v','>','^','>','v','v','<','>','v','<','<','<','<','v','<','^','v','>','^','<','^','^','>','>','>','^','<','v','<','v',
                                '>','<','>','v','v','>','v','^','v','^','<','>','>','<','>','>','>','>','<','^','^','>','v','v','>','v','<','^','^','^','>','>','v','^','v','^','<','^','^','>','v','^','^','>','v','^','<','^','v','>','v','<','>','>','v','^','v','^','<','v','>','v','^','^','<','^','^','v','v','<',
                                '<','<','v','<','^','>','>','^','^','^','^','>','>','>','v','^','<','>','v','v','v','^','>','<','v','<','<','<','>','^','^','^','v','v','^','<','v','v','v','>','^','>','v','<','^','^','^','^','v','<','>','^','>','v','v','v','v','>','<','>','>','v','^','<','<','^','^','^','^','^',
                                '^','>','<','^','>','<','>','>','>','<','>','^','^','<','<','^','^','v','>','>','>','<','^','<','v','>','^','<','v','v','>','>','v','>','>','>','^','v','>','<','>','^','v','>','<','<','<','<','v','>','>','v','<','v','<','v','>','v','v','v','>','^','<','>','<','<','>','^','>','<',
                                '^','>','>','<','>','^','v','<','>','<','^','v','v','v','<','^','^','<','>','<','v','<','<','<','<','<','>','<','^','v','<','<','<','>','<','<','<','^','^','<','v','<','^','^','^','>','<','^','>','>','^','<','v','^','>','<','<','<','^','>','>','^','v','<','v','^','v','<','v','^',
                                '>','^','>','>','^','v','>','v','v','>','^','<','<','^','v','<','>','>','<','<','>','<','<','v','<','<','v','>','<','>','v','<','^','v','v','<','<','<','>','^','^','v','^','>','^','^','>','>','>','<','<','^','v','>','>','v','^','v','>','<','^','^','>','>','^','<','>','v','v','^',
                                '<','>','<','^','^','>','^','^','^','<','>','<','v','v','v','v','v','^','v','<','v','<','<','>','^','v','<','v','>','v','<','<','^','>','<','<','>','<','<','>','<','<','<','^','^','<','<','<','^','<','<','>','>','<','<','>','<','^','^','^','>','^','^','<','>','^','>','v','<','>',
                                '^','^','>','v','v','<','^','v','^','v','<','v','v','>','^','<','>','<','v','<','^','v','>','^','^','^','>','>','>','^','^','v','v','v','^','>','v','v','v','<','>','>','>','^','<','^','>','>','>','>','>','^','<','<','^','v','>','^','v','v','v','<','>','^','<','>','<','<','v','>',
                                'v','^','^','>','>','>','<','<','^','^','<','>','>','^','v','^','<','v','^','v','v','<','>','v','^','<','<','>','^','<','^','v','^','v','>','<','^','<','<','<','>','<','<','^','<','v','>','<','v','<','>','v','v','>','>','v','>','<','v','^','<','v','v','<','>','v','^','<','<','^' };
	
	std::cout << "\n";
	
	long result = sumBoxGpsCoordinates(map, moves);
	
	if(result == 10092)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
