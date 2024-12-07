#include "day6.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>

bool Day6::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<char>> map;
	
	readInput(&map);
	
	// print
	std::cout << "MAP:\n";
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[";
		for(size_t j = 0; j < 16; j++)
		{
			std::cout << map[i][j];
		}
		std::cout << "] ... \n";
	}
	std::cout << "...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = countPositionsInTraversedMap(map);
	std::cout << ">>> GUARD POSITIONS: [" << result << "]\n";
		
	int result2 = countObstructionsLeadingToLoop(map);
	std::cout << ">>> LOOP OBJECT POSITIONS: [" << result2 << "]\n";	
		
	return true;
}

bool Day6::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = countPositionsInTraversedMap_test();
	if(ok) ok = countObstructionsLeadingToLoop_test();
	
	return ok;
}

/**
 * CODE
 */
int Day6::countPositionsInTraversedMap(std::vector<std::vector<char>> map)
{
	// traverse
	bool cont;
	
	int x;
	int y;
	int d;
	
	if(!locateGuard(&map, &x, &y, &d))
	{
		std::cout << "ERROR: guard not found\n";
		return false;
	}
	
	std::vector<std::vector<int>> coordinates;
	
	do
	{		
		cont = step(&map,&x,&y,&d);
		
		// save visited coordinates (exclude duplicates)		
		std::vector<int> cc = {x, y};	
		if(std::find(coordinates.begin(), coordinates.end(), cc) == coordinates.end()) coordinates.push_back(cc);	
				
	} while(cont);

	// count X-s
	int count = coordinates.size();
	
	return count;
}

int Day6::countObstructionsLeadingToLoop(std::vector<std::vector<char>> map)
{
	std::vector<std::vector<char>> map_copy;
	std::vector<std::vector<char>> map_before;
	
	std::ofstream logfile("log/day6.txt");
	if(!logfile.is_open())
	{
		std::cout << "ERROR: log file could not be opened\n";
		return 0;
	}	
	
	logfile << "DAY6:\n";
	
	int object_results_in_loop_count = 0;
	
	int x;
	int y;
	int d;
	int start_x;
	int start_y;
	int start_d;
	
	bool cont;
	
	// find all walking coordinates
	map_copy = map;
	locateGuard(&map_copy, &x, &y, &d);
	start_x = x;
	start_y = y;
	start_d = d;
	
	std::cout << "guard at [" << x << "," << y << "," << d << "]\n";
	
	std::vector<std::vector<int>> coordinates;
	coordinates.push_back(std::vector<int> {x, y, d});
	
	do
	{
		// do one step
		cont = step(&map_copy, &x, &y, &d);
				
		if(!cont) break;	// out of bounds, probably		
				
		// save visited coordinates (exclude duplicates)		
		std::vector<int> cc = {x, y};	
		if(std::find(coordinates.begin(), coordinates.end(), cc) == coordinates.end()) coordinates.push_back(cc);	
				
					
	} while(cont);
	
	
	// apply object to walking coordinates
	int i_max = (int)coordinates.size();
	for(int i = 0; i < i_max; i++)
	{

		int a = coordinates[i][0];
		int b = coordinates[i][1];
		
		map_copy = map;
		map_copy.at(b).at(a) = '#';
		
		std::cout << "[" << i << "/" << i_max << "] placed object at [" << a << "," << b << "]... ";
		logfile << "[" << i << "/" << i_max << "] placed object at [" << a << "," << b << "]... ";
		
		x = start_x;
		y = start_y;
		d = start_d;
		
		bool loop_detected = false;
	
		std::vector<std::vector<int>> steps_taken;
		
		do
		{	
			// do one step
			cont = step(&map_copy, &x, &y, &d);
				
			if(!cont) break;	// out of bounds, probably	
				
			// save the step (include duplicates)
			std::vector<int> cc = {x, y, d};
			if(std::find(steps_taken.begin(), steps_taken.end(), cc) != steps_taken.end())
			{
				loop_detected = true; // we're done when we're on the path we already traveled - including direction
				break;
			}
			else
			{
				steps_taken.push_back(cc);
			}
						
				
		} while(cont);
			
		if(loop_detected)
		{
			object_results_in_loop_count++;
			std::cout << "LOOP DETECTED --> count [" << object_results_in_loop_count << "]\n";
			logfile << "LOOP DETECTED --> count [" << object_results_in_loop_count << "]\n";
			//printMap(map_copy);
				
		}
		else
		{
			std::cout << "nothing\n";
			logfile << "nothing\n";
		}
			
		logfile.flush();
	}	
	
	logfile << "[" << object_results_in_loop_count << "] loops found\n";
	
	// close log
	logfile.close();
	
	
	
	return object_results_in_loop_count;
}

bool Day6::step(std::vector<std::vector<char>> * map_p, int * x, int * y, int * d)
{
	std::vector<std::vector<int>> dirs = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
			
	// make direction
	int dx = dirs[*d][0];
	int dy = dirs[*d][1];
	
	// check next step out of bounds
	int x_next = *x + dx;
	int y_next = *y + dy;
	int x_max = map_p->at(0).size();
	int y_max = map_p->size();
	
	if(x_next < 0 || x_next >= x_max || y_next < 0 || y_next >= y_max) return false;
	
	// check next step obstacle
	if(map_p->at(y_next).at(x_next) == '#')
	{
		// rotate right
		*d = (*d + 1) % 4;
	}
	else
	{
		// do step
		*x += dx;
		*y += dy;
	}
	
	return true;	
}

bool Day6::locateGuard(std::vector<std::vector<char>> * map_p, int * x, int * y, int * d)
{
	for(size_t a = 0; a < map_p->size(); a++)
	{
		for(size_t b = 0; b < map_p->at(a).size(); b++)
		{
			char g = map_p->at(a).at(b);
			int dir;
			
			if(g == '^' || g == '>' || g == 'v' || g == '<')
			{
				if(x != nullptr) *x = (int)b;
				if(y != nullptr) *y = (int)a;
				
				if(g == '^'){ dir = 0; }
				else if(g == '>'){ dir = 1; }
				else if(g == 'v'){ dir = 2; }
				else if(g == '<'){ dir = 3; }
				else { std::cout << "ERROR: should not happen\n"; return false; }
				
				if(d != nullptr) *d = dir;
				
				return true;
			}
		}
	}
	
	return false;
}

void Day6::printMap(std::vector<std::vector<char>> map)
{
	for(size_t a = 0; a < map.size(); a++)
	{
		for(size_t b = 0; b < map.at(a).size(); b++)
		{
			std::cout << map.at(a).at(b);
		}
		
		std::cout << "\n";
	}	
}

/**
 * INPUT
 */
bool Day6::readInput(std::vector<std::vector<char>> * map)
{
	std::string line;
	
	// load rules
	std::ifstream infile("data/day6.txt");

	map->clear();

	while(getline(infile, line))
	{
		std::vector<char> w;
		
		for(size_t i = 0; i < line.size(); i++)
		{
			w.push_back(line[i]);
		}
		
		map->push_back(w);
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day6::countPositionsInTraversedMap_test()
{
	std::vector<std::vector<char>> map = { {'.','.','.','.','#','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','#'},
                                           {'.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','#','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','#','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.'},
                                           {'.','#','.','.','^','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','#','.'},
                                           {'#','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','#','.','.','.'}, };
	
	std::cout << "\n";
	
	int result = countPositionsInTraversedMap(map);
	
	if(result == 41)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day6::countObstructionsLeadingToLoop_test()
{
	std::vector<std::vector<char>> map = { {'.','.','.','.','#','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','#'},
                                           {'.','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','#','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','#','.','.'},
                                           {'.','.','.','.','.','.','.','.','.','.'},
                                           {'.','#','.','.','^','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','.','.','#','.'},
                                           {'#','.','.','.','.','.','.','.','.','.'},
                                           {'.','.','.','.','.','.','#','.','.','.'}, };
	
	std::cout << "\n";
	
	int result = countObstructionsLeadingToLoop(map);
	
	if(result == 6)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
