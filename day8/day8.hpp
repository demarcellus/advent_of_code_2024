#pragma once

#include <vector>

class Day8
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<char>> * map);
		
		bool uniqueAntinodeLocations_test();
		bool uniqueAntinodeLocations2_test();
		
		int uniqueAntinodeLocations(std::vector<std::vector<char>> map);
		int uniqueAntinodeLocations2(std::vector<std::vector<char>> map);
		
		void printMap(std::vector<std::vector<char>> * map);

};
