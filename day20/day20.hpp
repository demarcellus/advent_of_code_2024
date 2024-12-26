#pragma once

#include <vector>

class Day20
{
	public:



		bool run();
		bool test();

	private:
	
		std::pair<int,int> findLocation(const std::vector<std::vector<char>>& map, const char c);
		std::vector<std::pair<int,int>> getPath(const std::vector<std::vector<char>>& map);
		std::vector<std::pair<int,int>> getLocationsInMapFromLocationWithDistance(const std::vector<std::vector<char>>& map, const std::pair<int,int>& loc, const int& distance);
	

		int saveAtLeastLimitPicosecondsCheats(const std::vector<std::vector<char>>& map, const int cheat_length, const int limit);
		
		void printMap(const std::vector<std::vector<char>>& map);


		bool saveAtLeastLimitPicosecondsCheats_test();
		bool saveAtLeastLimitPicosecondsCheats_test2();

		bool readInput(std::vector<std::vector<char>> * map);
		
};
