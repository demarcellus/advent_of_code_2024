#include "day4.hpp"
		
#include <iostream>
#include <fstream>

#include <algorithm>

bool Day4::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<char>> v;
	
	readInput(&v);
	
	// print
	
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[ ";
		
		for(size_t j = 0; j < 16; j++)
		{
			std::cout << v[i][j];
		}
		
		std::cout << " ... ]\n";
		
	}
	std::cout << " ... \n";
	

	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = findXMAS(v);

	std::cout << ">>> FOUND XMAS-S RESULT: [" << result << "]\n";
	
	int result2 = findX_MAS(v);
	
	std::cout << ">>> FOUND X_MAS-S RESULT: [" << result2 << "]\n";
	
	return true;
}

bool Day4::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = findXMAS_test();
	if(ok) ok = findX_MAS_test();
	
	return ok;
}

int Day4::findXMAS(std::vector<std::vector<char>> v)
{
	int count = 0;

	int x_max = v[0].size();
	int y_max = v.size();
	
	for(int x = 0; x < x_max; x++)
	{
		for(int y = 0; y < y_max; y++)
		{
			int found = searchByCoordinates(v,x,y);
			
			if(found > 0)
			{
				count += found;
				
				//std::cout << "found XMAS at [" << x << "," << y << "] --> count [" << count << "]\n";				
			}
		}
	}
	
	return count;
}

int Day4::findX_MAS(std::vector<std::vector<char>> v)
{
    int count = 0;

	int x_max = v[0].size();
	int y_max = v.size();
	
	for(int x = 0; x < x_max; x++)
	{
		for(int y = 0; y < y_max; y++)
		{
			bool found = searchMasByCoordinates(v,x,y);
			
			if(found)
			{
				count++;
				
				//std::cout << "found X_MAS at [" << x << "," << y << "] --> count [" << count << "]\n";				
			}
		}
	}
	
	return count;
}

int Day4::searchByCoordinates(std::vector<std::vector<char>> v, int x_start, int y_start)
{
	std::vector<char> word = {'X','M','A','S'};
	
	int found = 0;
	
	int x_max = v[0].size();
	int y_max = v.size();
	
	// if character 1 is not here return
	if(v[x_start][y_start] != word[0]) return 0;
	
	// loop over all search directions
	std::vector<int> x_dir = { -1, -1, -1, 0, 0, 1, 1, 1 };
    std::vector<int> y_dir = { -1, 0, 1, -1, 1, -1, 0, 1 };
	
	for(int dir = 0; dir < 8; dir++)
	{
		//std::cout << "[" << x_start << "," << y_start << "] checking dir [" << dir << "] --> ";
		
		int word_index;
		int x = x_start + x_dir[dir];
		int y = y_start + y_dir[dir];
		
		for(word_index = 1; word_index < (int)word.size(); word_index++)
		{
			// check if within v?
			if(x < 0) break;
			if(x >= x_max) break;
			if(y < 0) break;
			if(y >= y_max) break;
			
			// is the character in the word found?
			if(v[x][y] != word[word_index]) break;
			
			// next
			x += x_dir[dir];
			y += y_dir[dir];
		}
		
		// did we find the complete word?
		if(word_index == (int)word.size())
		{
			//std::cout << "found";
			found++;
		}
		else
		{
			//std::cout << "nothing";
		}
		
		//std::cout << "\n";
	}
	
	return found;
}

bool Day4::searchMasByCoordinates(std::vector<std::vector<char>> v, int x, int y)
{
	std::vector<char> word = {'M','A','S'};
	
	int x_max = v[0].size();
	int y_max = v.size();
	
	// if too close too sides
	if(x < 1)          return false;
	if(x >= x_max - 1) return false;
	if(y < 1)          return false;
	if(y >= y_max - 1) return false;
	
	// if character 1 is not here return
	if(v[x][y] != 'A') return false;
	
	// just check all options
	
	// rotation 0deg
	// M.S
	// .A. << we know this one is here
	// M.S
	if( (v[x - 1][y - 1] == 'M') && (v[x + 1][y - 1] == 'S') &&
	    (v[x - 1][y + 1] == 'M') && (v[x + 1][y + 1] == 'S') ) return true; 
	
	// rotation 90deg
	// M.M
	// .A. << we know this one is here
	// S.S
	if( (v[x - 1][y - 1] == 'M') && (v[x + 1][y - 1] == 'M') &&
	    (v[x - 1][y + 1] == 'S') && (v[x + 1][y + 1] == 'S') ) return true; 
	
	// rotation 180deg
	// S.M
	// .A. << we know this one is here
	// S.M
	if( (v[x - 1][y - 1] == 'S') && (v[x + 1][y - 1] == 'M') &&
	    (v[x - 1][y + 1] == 'S') && (v[x + 1][y + 1] == 'M') ) return true; 
	
	// rotation 270deg
	// S.S
	// .A. << we know this one is here
	// M.M
	if( (v[x - 1][y - 1] == 'S') && (v[x + 1][y - 1] == 'S') &&
	    (v[x - 1][y + 1] == 'M') && (v[x + 1][y + 1] == 'M') ) return true; 
	
	return false;
}

/**
 * INPUT
 */ 
bool Day4::readInput(std::vector<std::vector<char>> * v)
{
	std::ifstream infile("data/day4.txt");
	
	v->clear();
	std::string line;
	
	while(getline(infile, line))
	{
		std::vector<char> w;
		
		for(size_t i = 0; i < line.size(); i++)
		{
			w.push_back(line[i]);
		}
		
		v->push_back(w);
	}	
		
	return true;
}

/**
 * TESTS 
 */
bool Day4::findXMAS_test()
{
	std::vector<std::vector<char>> v = { {'M','M','M','S','X','X','M','A','S','M'},		
                                         {'M','S','A','M','X','M','S','M','S','A'},
                                         {'A','M','X','S','X','M','A','A','M','M'},
                                         {'M','S','A','M','A','S','M','S','M','X'},
                                         {'X','M','A','S','A','M','X','A','M','M'},
                                         {'X','X','A','M','M','X','X','A','M','A'},
                                         {'S','M','S','M','S','A','S','X','S','S'},
                                         {'S','A','X','A','M','A','S','A','A','A'},
                                         {'M','A','M','M','M','X','M','M','M','M'},
                                         {'M','X','M','X','A','X','M','A','S','X'} };
	
	int result = findXMAS(v);
	
	if(result == 18)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day4::findX_MAS_test()
{
	std::vector<std::vector<char>> v = { {'M','M','M','S','X','X','M','A','S','M'},		
                                         {'M','S','A','M','X','M','S','M','S','A'},
                                         {'A','M','X','S','X','M','A','A','M','M'},
                                         {'M','S','A','M','A','S','M','S','M','X'},
                                         {'X','M','A','S','A','M','X','A','M','M'},
                                         {'X','X','A','M','M','X','X','A','M','A'},
                                         {'S','M','S','M','S','A','S','X','S','S'},
                                         {'S','A','X','A','M','A','S','A','A','A'},
                                         {'M','A','M','M','M','X','M','M','M','M'},
                                         {'M','X','M','X','A','X','M','A','S','X'} };
	
	int result = findX_MAS(v);
	
	if(result == 9)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

