#pragma once

#include <vector>
#include <string>
#include <map>

class Robot
{
	public:
	
		std::map<std::pair<char,char>,std::string> keymap;
		char initial_position;
		
		char current_key;
	
		std::map<std::pair<char,char>, long> cache;
	
		Robot * robot_p;
		
		Robot(const char& _initial_position, const std::map<std::pair<char,char>,std::string>& _keymap);
		
		std::string requestedKey(const char c);
		long requestedKeyCount(const char c);
};

class Day21
{
	public:



		bool run();
		bool test();

	private:
	
		
		long getSumOfComlexities(const std::vector<std::string>& codes, const int& keypad_robots);

		bool getSumOfComlexities_test();

		bool readInput(std::vector<std::string> * codes);
		
};
