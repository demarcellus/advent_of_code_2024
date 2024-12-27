#include "day21.hpp"
		
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <map>

std::pair<int,int> findCharInMap(const std::vector<std::vector<char>>& map, const char c)
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

std::map<std::pair<char,char>,std::string> makeKeyPad(const std::vector<std::vector<char>>& map, const std::vector<char>& keys)
{
	std::pair<int,int> empty = findCharInMap(map, ' ');
	
	// prepare output
	std::map<std::pair<char,char>,std::string> output;
	
	for(const auto& from : keys)
	{
		const auto pos_from = findCharInMap(map, from);
		
		for(const auto& to : keys)
		{
			const auto pos_to = findCharInMap(map, to);
			
			const int delta_x = pos_to.first  - pos_from.first;
			const int delta_y = pos_to.second - pos_from.second;
		
			//std::cout << "[" << f << " to " << t << "] dx[" << delta_x << "] dy[" << delta_y << "] --> ";
			
			// group all moves (^^> instead of ^>^, the parent robot only needs to press A then for the second item)
			std::string x_str;
			std::string y_str;
			
			if(delta_x < 0)		 x_str = std::string(std::abs(delta_x),'<'); 
			else if(delta_x > 0) x_str = std::string(std::abs(delta_x),'>');
			
			if(delta_y < 0)		 y_str = std::string(std::abs(delta_y),'^'); 
			else if(delta_y > 0) y_str = std::string(std::abs(delta_y),'v');
			
			std::stringstream out_s;
	
			//if(pos_from.second == empty.second && pos_to.first == empty.first)
			
			if(pos_to.first > pos_from.first && std::pair<int,int>{pos_from.first, pos_to.second} != empty)
			{
				// y-first
				out_s << y_str << x_str;
			}
			else if(std::pair<int,int>{pos_to.first, pos_from.second} != empty)
			{
				// x-first
				out_s << x_str << y_str;
			}
			else
			{
				// don't care
				out_s << y_str << x_str;
			}
				
			out_s << 'A';
			
			//std::cout << "[" << out_s.str() << "]\n";
			
			output[{from,to}] = out_s.str();
		}
	}
	
	//std::cout << "Numeric Keypad Map:\n";
	//for(auto& [key, value] : output)
	//{
	//	std::cout << "[" << key.first << "," << key.second << "] --> [" << value << "]\n";
	//}
	
	return output;
}

std::map<std::pair<char,char>,std::string> makeNumericKeypadMap()
{
	// keypad definition
	const std::vector<std::vector<char>> map = { {'7', '8', '9'},
		                                         {'4', '5', '6'},
		                                         {'1', '2', '3'},
		                                         {' ', '0', 'A'} };
		                               
	const std::vector<char> keys = {'9','8','7','6','5','4','3','2','1','0','A'};
	
	return makeKeyPad(map, keys);
}

std::map<std::pair<char,char>,std::string> makeArrowKeypadMap()
{
	// keypad definition
	const std::vector<std::vector<char>> map = { {' ', '^', 'A'},
		                                         {'<', 'v', '>'} };
		                               
	const std::vector<char> keys = {'<','^','v','>','A'};
	
	return makeKeyPad(map, keys);
}

Robot::Robot(const char& _initial_position, const std::map<std::pair<char,char>,std::string>& _keymap)
{
	this->initial_position = _initial_position;
	this->current_key = _initial_position;
	this->keymap = _keymap;
	this->robot_p = nullptr;
}

std::string Robot::requestedKey(const char c)
{
	std::stringstream out_s;
	
	const std::pair<char, char> key = {this->current_key, c};
	const std::string req = this->keymap[key];
	this->current_key = c;
	
	for(auto& a : req)
	{
		if(this->robot_p != nullptr)
		{
			out_s << this->robot_p->requestedKey(a);
		}
		else
		{
			out_s << a;
		}		
		
	}
	
	//std::cout << "[" << c << "] --> [" << out_s.str() << "]\n";
	                                                    
	return out_s.str();
}

long Robot::requestedKeyCount(const char c)
{
	long count = 0;
	
	const std::pair<char, char> key = {this->current_key, c};
	const std::string req = this->keymap[key];
	this->current_key = c;
	
	if(this->cache.count(key) == 0)
	{ 
	
		for(auto& a : req)
		{
			if(this->robot_p != nullptr)
			{
				count += this->robot_p->requestedKeyCount(a);
			}
			else
			{
				count += 1;
			}		
		}
		
		this->cache[key] = count;
	}
	
	//std::cout << "[" << c << "] --> [" << out_s.str() << "]\n";
	                                                    
	return this->cache[key];
}

bool Day21::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::string> codes;
	
	readInput(&codes);
	
	// print
	std::cout << "CODES:\n";
	for(auto& code : codes)
	{
		std::cout << "[" << code << "]\n";
	}
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	long result = getSumOfComlexities(codes,2);
	std::cout << ">>> SUM OF COMPLEXITIES - 2 INTERMEDIATE: [" << result << "]\n";
	
	long result2 = getSumOfComlexities(codes,25);
	std::cout << ">>> SUM OF COMPLEXITIES - 25 INTERMEDIATE: [" << result2 << "]\n";
	
	
	// 182932 is too high
	// >>> 176452 is ok!

	return true;
}

bool Day21::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = getSumOfComlexities_test();

	return ok;
}

/**
 * CODE
 */
long Day21::getSumOfComlexities(const std::vector<std::string>& codes, const int& keypad_robots)
{
	// make instances and couple them
	Robot * robot_at_door_p      = new Robot('A', makeNumericKeypadMap());
	
	std::vector<Robot *> robot_ps;
	Robot * robot_prev_p = robot_at_door_p;
	
	for(int i = 0; i < keypad_robots; i++)
	{
		Robot * p = new Robot('A', makeArrowKeypadMap());
		robot_prev_p->robot_p = p;
		robot_prev_p = p;
		robot_ps.push_back(p);		
	}
	
	
	//Robot * robot_in_radiation_p = new Robot('A', makeArrowKeypadMap());
	//Robot * robot_in_cold_p      = new Robot('A', makeArrowKeypadMap());	
	
	// couple them
	//robot_at_door_p->robot_p      = robot_in_radiation_p;
	//robot_in_radiation_p->robot_p = robot_in_cold_p;
	//robot_in_cold_p->robot_p      = nullptr;
	
	//std::map<std::string,std::string> results;
	std::map<std::string,long> results;
	
	for(auto& code : codes)
	{
		//std::stringstream ss;
		long count = 0;
		
		for(auto& c : code)
		{
			//ss << robot_at_door_p->requestedKey(c);
			count += robot_at_door_p->requestedKeyCount(c);
		}
		
		//results[code] = ss.str();
		results[code] = count;
	}
	
	long sum = 0;
	
	std::cout << "Results:\n";
	for(auto& [code, result] : results)
	{
		long numeric_part_of_code = std::stol(code.substr(0,3));
		//long length_of_result = (long)result.size();
		long length_of_result = result;
		long complexity = numeric_part_of_code * length_of_result;
		
		sum += complexity;
		
		std::cout << "[" << code << "] --> [" << result << "] complexity [" << numeric_part_of_code << " * " << length_of_result << "] --> [" << complexity << "]\n";
	}
	
	return sum;
}

/**
 * INPUT
 */
bool Day21::readInput(std::vector<std::string> * codes)
{
	std::string line;
	
	std::ifstream infile("data/day21.txt");

	codes->clear();

	while(getline(infile, line))
	{
		codes->push_back(line);
	}	
		
	return true;
}


/**
 * TESTS 
 */
bool Day21::getSumOfComlexities_test()
{
	std::vector<std::string> codes = { "029A", "980A", "179A", "456A", "379A" };
	
	std::cout << "\n";
	
	int result = getSumOfComlexities(codes,2);
	
	if(result == 126384)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
