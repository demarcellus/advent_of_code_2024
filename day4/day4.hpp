#pragma once

#include <vector>

class Day4
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<char>> * v);
		
		int findXMAS(std::vector<std::vector<char>> v);
		int findX_MAS(std::vector<std::vector<char>> v);
		
		bool findXMAS_test();
		bool findX_MAS_test();
		
		int searchByCoordinates(std::vector<std::vector<char>> v, int x_start, int y_start);
		bool searchMasByCoordinates(std::vector<std::vector<char>> v, int x, int y);
};
