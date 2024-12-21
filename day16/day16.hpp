#pragma once

#include <vector>

struct Position
{
	int x;
	int y;
	int dir;
			
	bool equals(const Position& pos) const
	{
		return (x == pos.x && y == pos.y);
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

class Day16
{
	public:



		bool run();
		bool test();

	private:
	

	
		bool readInput(std::vector<std::vector<char>> * map);

		long solveMaze(std::vector<std::vector<char>> map);

		Position findStart(std::vector<std::vector<char>> * map);
		Position findEnd(std::vector<std::vector<char>> * map); 

		bool solveMaze_test();
		bool solveMaze_test2();
		
};
