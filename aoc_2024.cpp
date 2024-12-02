#include <iostream>


#include "day1/day1.hpp"
#include "day2/day2.hpp"

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
	std::cout << "TESTS:\n";
	if(ok) ok = day1.calculateDistance_test();
	if(ok) ok = day1.calculateSimilarityScore_test();

	if(!ok) return -1;

	// ACTUAL PROGRAM
	if(ok) ok = day1.run();

	if(!ok) return -1;
	*/
	
	/********
	 * DAY2 *
	 ********/
	Day2 day2;
	
	std::cout << "<<< DAY 2 >>>\n";
	
	// RUN TESTS
	std::cout << "TESTS:\n";
	if(ok) ok = day2.countSafeReports_test();
	if(ok) ok = day2.countSafeReportsProblemDampener_test();
	
	if(!ok) return -1;
	
	// ACTUAL PROGRAM
	if(ok) ok = day2.run();
	
	if(!ok) return -1;
	
	
	
	
	/********
	 * DONE *
	 ********/

	return 0;
}

/**
 * FUNCTIONS
 */

