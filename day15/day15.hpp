#pragma once

#include <vector>

class Object
{
	public:
		enum Type { Robot, Box, Box_Left, Box_Right, Wall, Empty};
	
		Type type;
		int x;
		int y;
		
		Object * coupled;
		
		Object(Type _type, int _x, int _y);
		void move(std::vector<Object *>& objects, char direction, bool move_coupled, int depth);
		
		bool canMove(std::vector<Object *>& objects, char direction, bool move_coupled, int depth);
};
	
class Day15
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<char>> * map, std::vector<char> * moves);

		long sumBoxGpsCoordinates(std::vector<std::vector<char>> map, std::vector<char> moves, bool second_warehouse);

		void printObjects(std::vector<std::vector<char>>& map, std::vector<Object *>& objects, Object& robot);

		void transform(std::vector<std::vector<char>>& map);

		bool sumBoxGpsCoordinates_small_test();
		bool sumBoxGpsCoordinates_large_test();
		bool sumBoxGpsCoordinates_second_small_test();
		bool sumBoxGpsCoordinates_second_large_test();

		
};
