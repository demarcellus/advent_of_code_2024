#include <iostream>


#include "day1/day1.hpp"
#include "day2/day2.hpp"
#include "day3/day3.hpp"

/**
 * FUNCTION PROTOTYPES
 */


/**
 * MAIN
 */
int main()
{	
	bool ok = true;
	
	/********
	 * DAY1 *
	 ********/
	/*
	Day1 day1;
	 
	std::cout << "<<< DAY 1 >>>\n";
	
	// RUN TESTS
	if(ok) ok = day1.test();

	if(!ok) return -1;

	// ACTUAL PROGRAM
	if(ok) ok = day1.run();

	if(!ok) return -1;
	*/
	
	/********
	 * DAY2 *
	 ********/
	/*
	Day2 day2;
	
	std::cout << "<<< DAY 2 >>>\n";
	
	// RUN TESTS
	if(ok) ok = day2.test();
	
	if(!ok) return -1;
	
	// ACTUAL PROGRAM
	if(ok) ok = day2.run();
	
	if(!ok) return -1;
	*/
	
	/********
	 * DAY3 *
	 ********/
	Day3 day3;
	
	std::cout << "<<< DAY 3 >>>\n";
	
	// RUN TESTS
	if(ok) ok = day3.test();
	
	if(!ok) return -1;
	
	// ACTUAL PROGRAM
	if(ok) ok = day3.run();
	
	if(!ok) return -1;
		
	
	/********
	 * DONE *
	 ********/

	return 0;
}

/**
 * FUNCTIONS
 */

