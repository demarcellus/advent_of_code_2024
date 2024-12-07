#pragma once

#include <vector>
#include <fstream>

class Day6
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<char>> * map);
		
		int countPositionsInTraversedMap(std::vector<std::vector<char>> map);
		int countObstructionsLeadingToLoop(std::vector<std::vector<char>> map);
		
		bool countPositionsInTraversedMap_test();
		bool countObstructionsLeadingToLoop_test();
				
		bool step(std::vector<std::vector<char>> * map_p, int * x, int * y, int * d);
		
		bool locateGuard(std::vector<std::vector<char>> * map_p, int * x, int * y, int * d);
		
		void printMap(std::vector<std::vector<char>> map);
};
