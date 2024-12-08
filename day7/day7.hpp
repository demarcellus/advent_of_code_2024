#pragma once

#include <vector>

class Day7
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<long>> * list);
		
		long sumOfTrueEquations(std::vector<std::vector<long>> list);
		long sumOfTrueEquations2(std::vector<std::vector<long>> list);
		
		bool sumOfTrueEquations_test();
		bool sumOfTrueEquations2_test();
		
		long concatenate(long a, long b);
		std::vector<int> decimalToBase(int base, int number, int length);
		std::vector<std::vector<int>> options(int len, int base);
		
		void printOptions(std::vector<std::vector<int>> o);
};
