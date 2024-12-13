#pragma once

#include <vector>

class Region
{
	public:
		char type;
		int  area;
		int  perimeter;
		int  sides;
		int  price;
	
		std::vector<std::vector<int>> coordinates_list;
	
		Region(char _type);
};

class Day12
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<char>> * garden);
	
		int calculateFencePrice(std::vector<std::vector<char>> garden);
		int calculateFencePriceSides(std::vector<std::vector<char>> garden);

		Region * isPartOfRegion(std::vector<Region *> * regions_list, int x, int y);
		void expandRegion(std::vector<std::vector<char>> * garden, Region * region, int x, int y);
		int regionPerimeter(std::vector<std::vector<char>> * garden, Region * region);
		int regionSides(std::vector<std::vector<char>> * garden, Region * region);


		void printRegion(std::vector<std::vector<char>> * garden, Region * region);


		bool calculateFencePrice_test();
		bool calculateFencePriceSides_test();

};
