#pragma once

#include <string>

class Day3
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::string * s);
		
		int filterUncorruptedMultiplyAndAdd(std::string s);
		int filterUncorruptedDoDontMultiplyAndAdd(std::string s);
		
		bool filterUncorruptedMultiplyAndAdd_test();
		bool filterUncorruptedDoDontMultiplyAndAdd_test();

};
