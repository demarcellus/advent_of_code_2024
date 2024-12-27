#pragma once

#include <vector>
#include <string>
#include <map>

class Day22
{
	public:



		bool run();
		bool test();

	private:
	
		
		unsigned long getNthIterationSecretNumberSum(const std::vector<unsigned long>& initial_secret_numbers, const int& iterations);

		bool operations_test();
		bool getNthIterationSecretNumberSum_test();

		bool readInput(std::vector<unsigned long> * initial_secret_numbers);
		
};
