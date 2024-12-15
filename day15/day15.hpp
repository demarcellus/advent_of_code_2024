#pragma once

#include <vector>

class Object
{
	public:
		enum Type { Robot, Box, Wall, Empty};
	
		Type type;
		int x;
		int y;
		
		Object(Type _type, int _x, int _y);
		bool move(std::vector<Object *>& objects, char direction, int depth);
};
	
class Day15
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<char>> * map, std::vector<char> * moves);

		long sumBoxGpsCoordinates(std::vector<std::vector<char>> map, std::vector<char> moves);


		void printObjects(std::vector<std::vector<char>>& map, std::vector<Object *>& objects, Object& robot);


		bool sumBoxGpsCoordinates_small_test();
		bool sumBoxGpsCoordinates_large_test();

		
};
