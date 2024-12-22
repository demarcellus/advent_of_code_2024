#pragma once

#include <vector>
#include <cmath>
#include <unordered_set>

struct Position
{
	int x;
	int y;
	int dir;
			
	bool equals(const Position& pos) const
	{
		return (x == pos.x && y == pos.y);
	}
	
	bool moveStraight(const Position& pos) const
	{
		return (dir == pos.dir);
	}
	
	bool moveBackwards(const Position& pos) const
	{
		return (std::abs(dir - pos.dir) == 2);
	}
	
	bool moveLeftRight(const Position& pos) const
	{
		return (std::abs(dir - pos.dir) == 1 || std::abs(dir - pos.dir) == 3);
	}
	
	bool operator == (const Position& pos) const
	{
		return (x == pos.x && y == pos.y && dir == pos.dir);
	}
	
	bool operator < (const Position& pos) const
	{
		return (x < pos.x || (x == pos.x && y < pos.y) || (x == pos.x && y == pos.y && dir < pos.dir));
	}
};

struct Hasher
{
	size_t operator()(const Position& pos) const
	{
		return (1000000 * pos.x) + (1000 * pos.y) + pos.dir;
	}
};

class Day16
{
	public:



		bool run();
		bool test();

	private:
	

	
		bool readInput(std::vector<std::vector<char>> * map);

		int solveMaze(const std::vector<std::vector<char>> map);
		int countTilesAlongBestPaths(const std::vector<std::vector<char>> map);

		Position findStart(const std::vector<std::vector<char>>& map);
		Position findEnd(const std::vector<std::vector<char>>& map); 

		std::pair<int,int> findBestPath(const std::vector<std::vector<char>>& map);
		std::vector<std::vector<std::vector<int>>> findBestScores(const std::vector<std::vector<char>>& map, const int best_path_score, const int end_dir);
		std::unordered_set<Position, Hasher> walkBestPaths(const std::vector<std::vector<char>>& map, std::vector<std::vector<std::vector<int>>>& best_score_map);
		int countUniquePositions(const std::unordered_set<Position, Hasher>& list);
	
	
		void printMap(const std::vector<std::vector<char>>& map, const std::unordered_set<Position, Hasher>& list);


		bool solveMaze_test();
		bool solveMaze_test2();
		
		bool countTilesAlongBestPaths_test();
		bool countTilesAlongBestPaths_test2();
		
};
