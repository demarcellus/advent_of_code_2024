#include "day12.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <algorithm>


Region::Region(char _type)
{
	this->type = _type;
	this->area = 0; 
	this->perimeter = 0;
	this->sides = 0;
	this->price = 0;
}

bool Day12::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<char>> garden;
	
	readInput(&garden);
	
	// print
	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			std::cout << garden[y][x];
		}
		
		std::cout << "\n";
	}
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = calculateFencePrice(garden);
	std::cout << ">>> FENCE PRICE PERIMETER: [" << result << "]\n";
	
	int result2 = calculateFencePriceSides(garden);
	std::cout << ">>> FENCE PRICE SIDES: [" << result2 << "]\n";
				
	return true;
}

bool Day12::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = calculateFencePrice_test();
	if(ok) ok = calculateFencePriceSides_test();
	
	return ok;
}

/**
 * CODE
 */
int Day12::calculateFencePrice(std::vector<std::vector<char>> garden)
{
	std::vector<Region *> regions_list;
	
	int total_price = 0;
	
	for(int y = 0; y < (int)garden.size(); y++)
	{
		for(int x = 0; x < (int)garden[y].size(); x++)
		{
			Region * p = isPartOfRegion(&regions_list, x, y);
			
			if(p == nullptr)
			{
				p = new Region(garden[y][x]);
				expandRegion(&garden, p, x, y);
				regions_list.push_back(p);
				
				//std::cout << "REGION MAP:\n";
				//printRegion(&garden, p);
				
				p->area = p->coordinates_list.size();
				p->perimeter = regionPerimeter(&garden, p);
				p->price = p->area * p->perimeter;
				
				//std::cout << "REGION AREA: [" << p->area  << "]\n";
				//std::cout << "REGION PERIMETER: [" << p->perimeter << "]\n";
				//std::cout << "REGION PRICE: [" << p->price << "]\n";
				//std::cout << "\n";
				
				total_price += p->price;
			}
		}
	}
	
	return total_price;
}

int Day12::calculateFencePriceSides(std::vector<std::vector<char>> garden)
{
	std::vector<Region *> regions_list;
	
	int total_price = 0;
	
	for(int y = 0; y < (int)garden.size(); y++)
	{
		for(int x = 0; x < (int)garden[y].size(); x++)
		{
			Region * p = isPartOfRegion(&regions_list, x, y);
			
			if(p == nullptr)
			{
				p = new Region(garden[y][x]);
				expandRegion(&garden, p, x, y);
				regions_list.push_back(p);
				
				//std::cout << "REGION MAP:\n";
				//printRegion(&garden, p);
				
				p->area = p->coordinates_list.size();
				p->sides = regionSides(&garden, p);
				p->price = p->area * p->sides;
				
				//std::cout << "REGION AREA: [" << p->area  << "]\n";
				//std::cout << "REGION SIDES: [" << p->sides << "]\n";
				//std::cout << "REGION PRICE: [" << p->price << "]\n";
				//std::cout << "\n";
				
				total_price += p->price;
			}
		}
	}
	
	return total_price;
}

Region * Day12::isPartOfRegion(std::vector<Region *> * regions_list, int x, int y)
{
	for(int i = 0; i < (int)regions_list->size(); i++)
	{
		Region * p = regions_list->at(i);
		
		std::vector<int> coordinates = {x,y};
		if(std::find(p->coordinates_list.begin(), p->coordinates_list.end(), coordinates) != p->coordinates_list.end())
		{
			return p;
		}
	}
	
	return nullptr;
}

void Day12::expandRegion(std::vector<std::vector<char>> * garden, Region * region, int x, int y)
{
	int x_max = (int)garden->at(0).size();
	int y_max = (int)garden->size();
	
	// check out of bounds
	if(x < 0 || x >= x_max || y < 0 || y >= y_max) return;
	
	if(garden->at(y).at(x) != region->type) return;
	
	// lookup if already in region
	std::vector<int> coordinates = {x,y};
	if(std::find(region->coordinates_list.begin(), region->coordinates_list.end(), coordinates) == region->coordinates_list.end())
	{
		region->coordinates_list.push_back(coordinates);
		
		// expand search
		std::vector<std::vector<int>> directions = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		for(auto direction : directions)
		{
			expandRegion(garden, region, x + direction[0], y + direction[1]);
		}
	}
}

int Day12::regionPerimeter(std::vector<std::vector<char>> * garden, Region * region)
{
	int x_max = (int)garden->at(0).size();
	int y_max = (int)garden->size();
	
	int perimeter = 0;
	
	for(auto coordinates : region->coordinates_list)
	{
		int perimeter_for_coordinates = 0;
				
		std::vector<std::vector<int>> directions = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		for(auto direction : directions)
		{
			int x_next = coordinates[0] + direction[0];
			int y_next = coordinates[1] + direction[1];
			
			// check if within bounds
			if(x_next < 0 || x_next >= x_max || y_next < 0 || y_next >= y_max) 
			{
				perimeter_for_coordinates++;
				continue;
			}
			
			// check if within region
			std::vector<int> cc = {x_next, y_next};
			if(std::find(region->coordinates_list.begin(), region->coordinates_list.end(), cc) == region->coordinates_list.end())
			{
				perimeter_for_coordinates++;
			}
		}
		
		perimeter += perimeter_for_coordinates;
	}
	
	return perimeter;
}

int Day12::regionSides(std::vector<std::vector<char>> * garden, Region * region)
{
	int sides = 0;
	
	for(auto coordinates : region->coordinates_list)
	{
				
		std::vector<std::vector<std::vector<int>>> corner_conditions = { { {0,  0, -1}, {0, -1,  0} },                 // outer upper left corner  : not up, not left
															      	     { {0,  0, -1}, {0,  1,  0} },                 // outer upper right corner : not up, not right
																         { {0,  0,  1}, {0, -1,  0} },                 // outer lower left corner  : not down, not left
																         { {0,  0,  1}, {0,  1,  0} },                 // outer lower right corner : not down, not right
																         { {0, -1, -1}, {1,  0, -1}, {1, -1,  0} },    // inner upper left corner  : not upleft, up, left
																         { {0,  1, -1}, {1,  0, -1}, {1,  1,  0} },    // inner upper right corner : not upright, up, right
																         { {0, -1,  1}, {1,  0,  1}, {1, -1,  0} },    // inner lower left corner  : not lowleft, down, left
																         { {0,  1,  1}, {1,  0,  1}, {1,  1,  0} } };  // inner lower right corner : not lowright, down, right
																         
		//int i = 0;
												
		//std::cout << "position [" << coordinates[0] << "," << coordinates[1] << "]\n";
																         
		for(auto directions : corner_conditions)
		{
			//std::cout << "checking corner condition [" << (i++) << "]\n";
			
			bool ok = true;
			
			// all directions should be satisfied
			for(auto direction : directions)
			{
				//std::cout << "> direction [" << direction[0] << "] [" << direction[1] << "," << direction[2] << "]\n";
				
				bool negate = (direction[0] == 0?true:false);
				int x_next = coordinates[0] + direction[1];
				int y_next = coordinates[1] + direction[2];
				
				//std::cout << ">> position next [" << x_next << "," << y_next << "]\n";
						
				// check if within region
				std::vector<int> cc = {x_next, y_next};
				bool in_region = std::find(region->coordinates_list.begin(), region->coordinates_list.end(), cc) != region->coordinates_list.end();
				
				//std::cout << ">>> [" << (in_region?"in":"out of") << "] region\n";
				
				if(negate) in_region = !in_region;
				
				if(in_region)
				{
					continue;
				}
			
				//std::cout << ">>> region failed\n";
				
				ok = false;
				break;
			}
			
			//std::cout << (ok?"all conditions satisfied":"conditions not satisfied") << "\n";
			
			if(ok) 
			{
				//std::cout << "sides++\n";
				sides++;
			}
		}
	}
	
	return sides;
}

void Day12::printRegion(std::vector<std::vector<char>> * garden, Region * region)
{
	std::vector<std::vector<char>> map(garden->size(), std::vector<char>(garden->at(0).size(), '.'));
	
	for(auto coordinates : region->coordinates_list)
	{
		map[coordinates[1]][coordinates[0]] = region->type;
	}
	
	for(int y = 0; y < (int)map.size(); y++)
	{
		for(int x = 0; x < (int)map[0].size(); x++)
		{
			std::cout << map[y][x];
		}
		std::cout << "\n";
	}
}

/**
 * INPUT
 */
bool Day12::readInput(std::vector<std::vector<char>> * garden)
{
	std::string line;
	
	std::ifstream infile("data/day12.txt");

	garden->clear();

	while(getline(infile, line))
	{
		std::vector<char> w;
		
		for(size_t i = 0; i < line.size(); i++)
		{
			w.push_back(line[i]);
		}
		
		garden->push_back(w);
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day12::calculateFencePrice_test()
{
	std::vector<std::vector<char>> garden = { {'R','R','R','R','I','I','C','C','F','F'},
		                                      {'R','R','R','R','I','I','C','C','C','F'},
		                                      {'V','V','R','R','R','C','C','F','F','F'},
		                                      {'V','V','R','C','C','C','J','F','F','F'},
		                                      {'V','V','V','V','C','J','J','C','F','E'},
		                                      {'V','V','I','V','C','C','J','J','E','E'},
		                                      {'V','V','I','I','I','C','J','J','E','E'},
		                                      {'M','I','I','I','I','I','J','J','E','E'},
		                                      {'M','I','I','I','S','I','J','E','E','E'},
		                                      {'M','M','M','I','S','S','J','E','E','E'} };
	
	std::cout << "\n";
	
	long result = calculateFencePrice(garden);
	
	if(result == 1930)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day12::calculateFencePriceSides_test()
{
	std::vector<std::vector<char>> garden = { {'R','R','R','R','I','I','C','C','F','F'},
		                                      {'R','R','R','R','I','I','C','C','C','F'},
		                                      {'V','V','R','R','R','C','C','F','F','F'},
		                                      {'V','V','R','C','C','C','J','F','F','F'},
		                                      {'V','V','V','V','C','J','J','C','F','E'},
		                                      {'V','V','I','V','C','C','J','J','E','E'},
		                                      {'V','V','I','I','I','C','J','J','E','E'},
		                                      {'M','I','I','I','I','I','J','J','E','E'},
		                                      {'M','I','I','I','S','I','J','E','E','E'},
		                                      {'M','M','M','I','S','S','J','E','E','E'} };
	
	std::cout << "\n";
	
	long result = calculateFencePriceSides(garden);
	
	if(result == 1206)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
