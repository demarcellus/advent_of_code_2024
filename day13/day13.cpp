#include "day13.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <algorithm>

#include <climits>
#include <cstdio>

bool Day13::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<std::pair<int,int>>> machines;
	
	readInput(&machines);
	
	// print
	for(int index = 0; index < 5; index++)
	{
		std::cout << "MACHINE [" << index << "] BUTTON_A [" << machines[index][0].first << "," << machines[index][0].first << "] BUTTON_B [" << machines[index][1].first << "," << machines[index][1].first << "] PRIZE [" << machines[index][2].first << "," << machines[index][2].first << "]\n";
	}
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = optimizeMachines2(machines, false);
	std::cout << ">>> MINIMAL TOKENS NEEDED: [" << result << "]\n";

	long long result2 = optimizeMachines2(machines, true);
	std::cout << ">>> MINIMAL TOKENS NEEDED 2: [" << result2 << "]\n";
				
	return true;
}

bool Day13::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = optimizeMachines_test();
	if(ok) ok = optimizeMachines2_test();
	
	return ok;
}

/**
 * CODE
 */
int Day13::optimizeMachines(std::vector<std::vector<std::pair<int,int>>> machines)
{
	//
	// yeah this doesn't work for very large numbers :-D 
	// ... and they know it
	//
	
	int a_max_presses = 100;
	int b_max_presses = 100;
	
	int total_token_cost = 0;
	
	for(auto machine : machines)
	{	
		int a_x_advance = machine[0].first;
		int a_y_advance = machine[0].second;
		int b_x_advance = machine[1].first;
		int b_y_advance = machine[1].second;
		int prize_x     = machine[2].first;
		int prize_y     = machine[2].second;
		
		int min_token_cost = INT_MAX;
		
		for(int a = 0; a < a_max_presses; a++)
		{
			for(int b = 0; b < b_max_presses; b++)
			{
				if( (a * a_x_advance + b * b_x_advance) == prize_x &&
				    (a * a_y_advance + b * b_y_advance) == prize_y )
				{
					int token_cost = (3 * a) + b;
					
					if(token_cost < min_token_cost)
					{
						min_token_cost = token_cost;
					}
				}
			}
		}
		
		if(min_token_cost < INT_MAX)
		{		
			total_token_cost += min_token_cost;
		}
		
		
	}
	
	return total_token_cost;
}

long long Day13::optimizeMachines2(std::vector<std::vector<std::pair<int,int>>> machines, bool increment)
{
	//
	// just going long long doesn't solve it...
	// ... lets try math
	//
	// we know the following
	// (1) prize_x = a_presses * a_x_advance + b_presses * b_x_advance
	// (2) prize_y = a_presses * a_y_advance + b_presses * b_y_advance
	//
	// two equations with two unknowns, nice, we can solve for this
	// (3) a_presses = (prize_x - b_presses * b_x_advance) / a_x_advance
	// (4) a_presses = (prize_y - b_presses * b_y_advance) / a_y_advance
	//
	// so now, solve for b_presses -->
	// (5) (prize_x - b_presses * b_x_advance) / a_x_advance = (prize_y - b_presses * b_y_advance) / a_y_advance
	// (6) b_presses = (prize_x * a_y_advance - prize_y * a_x_advance) / (b_x_advance * a_y_advance - a_x_advance * b_y_advance)
	// 
	// and input this in (1) or (2) gives
	// (7) a_presses = (prize_x - b_presses * b_x_advance) / a_x_advance
	//
	// check if results are integers and we have a solution
	
	long long total_token_cost = 0;
	
	int i = 0;
	
	for(auto machine : machines)
	{	
		std::cout << "MACHINE [" << (i++) << "]\n";
		
		double a_x_advance = machine[0].first;
		double a_y_advance = machine[0].second;
		double b_x_advance = machine[1].first;
		double b_y_advance = machine[1].second;
		double prize_x     = machine[2].first  + (increment?10000000000000.0:0.0);
		double prize_y     = machine[2].second + (increment?10000000000000.0:0.0);
		
		double b_presses = (prize_x * a_y_advance - prize_y * a_x_advance) / (b_x_advance * a_y_advance - a_x_advance * b_y_advance);
		
		long long b_presses_int = static_cast<long long>(b_presses);
		bool b_presses_is_integer = (b_presses == b_presses_int);
		
		if(!b_presses_is_integer)
		{
			std::cout << "> b_presses is not an integer [" << b_presses << "] --> no solution\n";
			continue;
		}
	
		double a_presses = (prize_x - b_presses * b_x_advance) / a_x_advance;
		
		long long a_presses_int = static_cast<long long>(a_presses);
		bool a_presses_is_integer = (a_presses == a_presses_int);
		
		if(!a_presses_is_integer)
		{
			std::cout << "> a_presses is not an integer [" << b_presses << "] --> no solution\n";
			continue;
		}
		
		long long token_cost = a_presses_int * 3 + b_presses_int;
		
		std::cout << "> solution found [" << a_presses_int << "] [" << b_presses_int << "] cost [" << token_cost << "]\n";
		
		total_token_cost += token_cost;
	}
		
	return total_token_cost;
}

/**
 * INPUT
 */
bool Day13::readInput(std::vector<std::vector<std::pair<int,int>>> * machines)
{
	std::string line;
	
	std::ifstream infile("data/day13.txt");

	machines->clear();
	
	std::vector<std::pair<int,int>> machine;

	int line_number = 0;
	while(getline(infile, line))
	{
		int line_type = line_number % 4;	
		int a,b;
		
		switch(line_type)
		{
			case 0:
		
				if(std::sscanf(line.c_str(),"Button A: X%d, Y%d",&a,&b) != 2){ std::cout << "READ ERROR"; return false; }
				machine.push_back({a,b});
							
				break;
			case 1:
			
				if(std::sscanf(line.c_str(),"Button B: X%d, Y%d",&a,&b) != 2){ std::cout << "READ ERROR"; return false; }
				machine.push_back({a,b});
			
				break;
			case 2:
			
				if(std::sscanf(line.c_str(),"Prize: X=%d, Y=%d",&a,&b) != 2){ std::cout << "READ ERROR"; return false; }
				machine.push_back({a,b});
				machines->push_back(machine);
				machine.clear();
			
				break;
			default:
				break;
		}
		
		
		line_number++;
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day13::optimizeMachines_test()
{
	std::vector<std::vector<std::pair<int,int>>> machines = { { {94,34}, {22,67}, {8400,5400} },
		                                                      { {26,66}, {67,21}, {12748,12176} },
		                                                      { {17,86}, {84,37}, {7870, 6450} },
		                                                      { {69,23}, {27,71}, {18641, 10279} } };
	
	std::cout << "\n";
	
	long result = optimizeMachines2(machines, false);
	
	if(result == 480)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day13::optimizeMachines2_test()
{
	std::vector<std::vector<std::pair<int,int>>> machines = { { {94,34}, {22,67}, {8400,5400} },
		                                                      { {26,66}, {67,21}, {12748,12176} },
		                                                      { {17,86}, {84,37}, {7870, 6450} },
		                                                      { {69,23}, {27,71}, {18641, 10279} } };
	
	std::cout << "\n";
	
	long long result = optimizeMachines2(machines, true);
	
	if(result == 875318608908)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
