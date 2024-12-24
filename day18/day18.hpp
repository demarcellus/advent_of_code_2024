#pragma once

#include <vector>
#include <unordered_set>
#include <cstddef>

struct Hasher
{
	size_t operator()(const std::pair<int,int>& p) const
	{
		return (1000 * p.first) + p.second;
	}
};

class Day18
{
	public:



		bool run();
		bool test();

	private:
	
	
		bool readInput(std::vector<std::pair<int,int>> * byte_positions);

		int minimumStepsToExit(const unsigned int size_x, const unsigned int size_y, const std::vector<std::pair<int,int>>& byte_positions, const unsigned int bytes_fallen);
		std::pair<int,int> firstBlockingByte(const unsigned int size_x, const unsigned int size_y, const std::vector<std::pair<int,int>>& byte_positions);

		std::vector<std::vector<char>> dropBytes(const unsigned int size_x, const unsigned int size_y, const std::vector<std::pair<int, int>>& byte_positions, const unsigned int bytes_fallen);
		
		int findBestPathScore(const std::vector<std::vector<char>>& map);
		std::vector<std::vector<int>> findBestScores(const std::vector<std::vector<char>>& map, const int best_path_score);
		std::unordered_set<std::pair<int,int>, Hasher> walkBestPath(const std::vector<std::vector<char>>& map, std::vector<std::vector<int>>& best_score_map);


		void printMap(const std::vector<std::vector<char>>& map, const std::unordered_set<std::pair<int, int>, Hasher>& list);


		bool minimumStepsToExit_test();
		bool firstBlockingByte_test();
		
};
