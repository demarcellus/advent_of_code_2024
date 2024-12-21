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
	
	this->coupled = nullptr;
}

void Object::move(std::vector<Object *>& objects, char direction, bool move_coupled, int depth)
{
	int  x_next;
	int  y_next;
	
	//std::string header;
	//for(int i = 0; i < depth; i++)
	//{
	//	header.append(">");
	//}
	
	// coupled
	if(move_coupled && (direction == '^' || direction == 'v') && this->coupled != nullptr)
	{
		//std::cout << header << "MOVE: Coupled vertical move\n";
		this->coupled->move(objects, direction, false, depth);
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
			std::cout << "MOVE: MOVE ERROR";
			return;
	}
	
	for(auto object : objects)
	{
		if(object->x == x_next && object->y == y_next)
		{
			//std::cout << header << "MOVE: Object found at [" << x_next << "," << y_next << "]\n";

			object->move(objects, direction, true, depth + 1);
			break;
		}
	}
	
	//if(!next_found)
	//{
	//	std::cout << header << "MOVE: No object found at [" << x_next << "," << y_next << "]\n";
	//}
	
	//std::cout << header << "MOVE: No object found at [" << x_next << "," << y_next << "] or next did move\n";
		
	this->x = x_next;
	this->y = y_next;

}

bool Object::canMove(std::vector<Object *>& objects, char direction, bool move_coupled, int depth)
{
	int  x_next;
	int  y_next;
	
	//std::string header;
	//for(int i = 0; i < depth; i++)
	//{
	//	header.append(">");
	//}
	
	// i'm a wall
	if(this->type == Object::Type::Wall)
	{
		//std::cout << header << "CHECK: I'm a wall - no move\n";
		return false;
	}
	
	// move indicators
	bool coupled_can_move = true;
	
	// move coupled vertical...
	if(move_coupled && (direction == '^' || direction == 'v') && this->coupled != nullptr)
	{
		//std::cout << header << "CHECK: Checking coupled vertical move\n";
		coupled_can_move = this->coupled->canMove(objects, direction, false, depth);
		//std::cout << header << "CHECK: Coupled object " << (coupled_can_move?"can":"cannot") << " move\n";
	}
	
	// exit early
	if(!coupled_can_move) return false;
	
	// find next coordinates
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
			std::cout << "CHECK: MOVE ERROR";
			return false;
	}
	
	bool next_found = false;
	bool next_can_move = false;
	
	for(auto object : objects)
	{
		if(object->x == x_next && object->y == y_next)
		{
			//std::cout << header << "CHECK: Object found at [" << x_next << "," << y_next << "]\n";
			
			next_found = true;
			next_can_move = object->canMove(objects, direction, true, depth + 1);
			break;
		}
	}
	
	if(!next_found || next_can_move)
	{
		//std::cout << header << "CHECK: No object found at [" << x_next << "," << y_next << "] or next can move\n";
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
	
	long result = sumBoxGpsCoordinates(map, moves, false);
	std::cout << ">>> GPS COORDINATES SUM: [" << result << "]\n";
		
	transform(map);	
		
	long result2 = sumBoxGpsCoordinates(map, moves, true);
	std::cout << ">>> SECOND WAREHOUSE GPS COORDINATES SUM: [" << result2 << "]\n";	
					
	return true;
}

bool Day15::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = sumBoxGpsCoordinates_small_test();
	if(ok) ok = sumBoxGpsCoordinates_large_test();
	if(ok) ok = sumBoxGpsCoordinates_second_small_test();
	if(ok) ok = sumBoxGpsCoordinates_second_large_test();
	
	return ok;
}

/**
 * CODE
 */
long Day15::sumBoxGpsCoordinates(std::vector<std::vector<char>> map, std::vector<char> moves, bool second_warehouse)
{
	std::vector<Object *> objects;
	Object robot = Object(Object::Type::Robot, -1, -1);
	
	Object * o1 = nullptr;
	Object * o2 = nullptr;
	
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
					
				case '[':
					
					o1 = new Object(Object::Type::Box_Left,  x,     y);
					o2 = new Object(Object::Type::Box_Right, x + 1, y);	// ignore case "]" because they are always in pairs
					
					o1->coupled = o2;
					o2->coupled = o1;
					
					objects.push_back(o1);
					objects.push_back(o2);
					
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

	//if(second_warehouse) return -1;
	
	// go move!
	//int i = 1;
	for(auto move : moves)
	{
		//std::cout << "[" << (i++) << "] MOVE [" << move << "]\n";
		
		if(robot.canMove(objects, move, true, 0))
		{
			robot.move(objects, move, true, 0);
		}

		//printObjects(map, objects, robot);	
		//std::cout << "\n";		
	}
	
	
	int coordinate_sum = 0;
	for(auto object : objects)
	{
		if(object->type == Object::Type::Box || object->type == Object::Type::Box_Left)
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
		else if(object->type == Object::Type::Box_Left) val = '[';
		else if(object->type == Object::Type::Box_Right) val = ']';
		
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

void Day15::transform(std::vector<std::vector<char>>& map)
{
	std::vector<std::vector<char>> map2;
	
	for(int y = 0; y < (int)map.size(); y++)
	{
		std::vector<char> v;
		
		for(int x = 0; x < (int)map[y].size(); x++)
		{
			char c = map[y][x];
			
			switch(c)
			{
				case 'O':
					v.push_back('[');
					v.push_back(']');
					break;
				case '@':
					v.push_back('@');
					v.push_back('.');
					break;
				default:
					v.push_back(c);
					v.push_back(c);
					break;
			}
		}
		map2.push_back(v);
	}
	
	map = map2;
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
	
	long result = sumBoxGpsCoordinates(map, moves, false);
	
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
	
	long result = sumBoxGpsCoordinates(map, moves, false);
	
	if(result == 10092)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day15::sumBoxGpsCoordinates_second_small_test()
{
	std::vector<std::vector<char>> map = { {'#','#','#','#','#','#','#'},
		                                   {'#','.','.','.','#','.','#'},
		                                   {'#','.','.','.','.','.','#'},
		                                   {'#','.','.','O','O','@','#'},
		                                   {'#','.','.','O','.','.','#'},
		                                   {'#','.','.','.','.','.','#'},
		                                   {'#','#','#','#','#','#','#'} };
	
	std::vector<char> moves = {'<','v','v','<','<','^','^','<','<','^','^'};
	
	std::cout << "\n";
	
	transform(map);
	
	long result = sumBoxGpsCoordinates(map, moves, true);
	
	if(result == 618)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day15::sumBoxGpsCoordinates_second_large_test()
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
	
	transform(map);
	
	long result = sumBoxGpsCoordinates(map, moves, true);
	
	if(result == 9021)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
