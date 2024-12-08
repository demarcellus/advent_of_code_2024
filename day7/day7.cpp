#include "day7.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath> 

#include <algorithm>

bool Day7::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<long>> list;
	
	readInput(&list);
	
	// print
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[";
		for(size_t j = 0; j < list[i].size(); j++)
		{
			std::cout << list[i][j] << " ";
		}
		std::cout << "]\n";
	}
	std::cout << "...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	long result = sumOfTrueEquations(list);
	std::cout << ">>> SUM OF EQUATIONS (+ and *): [" << result << "]\n";
		
	long result2 = sumOfTrueEquations2(list);
	std::cout << ">>> SUM OF EQUATIONS (+ and * and ||): [" << result2 << "]\n";	
		
	return true;
}

bool Day7::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = sumOfTrueEquations_test();
	if(ok) ok = sumOfTrueEquations2_test();
	
	return ok;
}

/**
 * CODE
 */
long Day7::sumOfTrueEquations(std::vector<std::vector<long>> list)
{
	long sum = 0;
	int i = 0;
	
	for(auto item : list)
	{
		std::cout << "ENTRY " << i++ << " --> ";
		
		bool match = false;
		std::vector<std::vector<int>> o = options(item.size() - 2, 2);
		
		// loop over all options
		for(auto option : o)
		{
			long result = item[1];
			for(int i = 0; i < (int)option.size(); i++)
			{
				if(option[i] == 0)
				{	// sum
					result += item[i + 2];
				}
				else if(option[i] == 1)
				{	// multiply
					result *= item[i + 2];
				}
			}
			
			if(result == item[0]) match = true;
		}
		
		if(match) sum += item[0];
		std::cout << "[" << (match?"MATCH":"NO MATCH") << "] sum [" << sum << "]\n";
	}
	
	return sum;
}

long Day7::sumOfTrueEquations2(std::vector<std::vector<long>> list)
{
	long sum = 0;
	int i = 0;
	
	for(auto item : list)
	{
		std::cout << "ENTRY " << i++ << " --> ";
		
		//std::cout << "  [";
		//for(auto value : item)
		//{
		//	std::cout << value << " ";
		//}
		//std::cout << "]\n";
		
		bool match = false;
		std::vector<std::vector<int>> o = options(item.size() - 2, 3);
		
		// loop over all options
		for(auto option : o)
		{
			long result = item[1];
			for(int i = 0; i < (int)option.size(); i++)
			{
				if(option[i] == 0)
				{	// sum
					result += item[i + 2];
				}
				else if(option[i] == 1)
				{	// multiply
					result *= item[i + 2];
				}
				else if(option[i] == 2)
				{	// concatenate
					result = concatenate(result, item[i + 2]);
				}
			}
			
			if(result == item[0]) match = true;
		}
		
		if(match) sum += item[0];
		std::cout << "RESULT [" << (match?"MATCH":"NO MATCH") << "] SUM [" << sum << "]\n";
	}
	
	return sum;
}

long Day7::concatenate(long a, long b)
{
	std::string result;
	
	result += std::to_string(a);
	result += std::to_string(b);
	
	return std::stol(result);
}

std::vector<int> Day7::decimalToBase(int base, int number, int length)
{
	int index = 0;
	std::vector<int> result(length,0);
	
	while(number > 0)
	{
		result[index++] = number % base;
		number /= base;
	}
	
	std::reverse(result.begin(), result.end()); // not necessary, this order works better debugging
	
	return result;
}

std::vector<std::vector<int>> Day7::options(int len, int base)
{
	std::vector<std::vector<int>> options;
	
	for(int i = 0; i < std::pow(base,len); i++)
	{		
		options.push_back(decimalToBase(base,i,len));
	}
	
	//printOptions(options);
	
	return options;
}

void Day7::printOptions(std::vector<std::vector<int>> o)
{
	for(auto item : o)
	{
		std::cout << "[";
		for(auto val : item)
		{
			std::cout << val;
		}
		std::cout << "]\n";
	}	
}


/**
 * INPUT
 */
bool Day7::readInput(std::vector<std::vector<long>> * list)
{
	std::string line;
	
	// load rules
	std::ifstream infile("data/day7.txt");

	list->clear();

	while(getline(infile, line))
	{
		std::stringstream ss(line);
		std::string tok;
		long val;
		std::vector<long> w;
		
		while(getline(ss, tok, ' '))
		{
			val = stol(tok);
			w.push_back(val);
		}		
		
		list->push_back(w);
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day7::sumOfTrueEquations_test()
{
	std::vector<std::vector<long>> list = { {190, 10, 19},
                                            {3267, 81, 40, 27},
                                            {83, 17, 5},
                                            {156, 15, 6},
                                            {7290, 6, 8, 6, 15},
                                            {161011, 16, 10, 13},
                                            {192, 17, 8, 14},
                                            {21037, 9, 7, 18, 13},
                                            {292, 11, 6, 16, 20} };
	
	std::cout << "\n";
	
	long result = sumOfTrueEquations(list);
	
	if(result == 3749)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day7::sumOfTrueEquations2_test()
{
	std::vector<std::vector<long>> list = { {190, 10, 19},
                                            {3267, 81, 40, 27},
                                            {83, 17, 5},
                                            {156, 15, 6},
                                            {7290, 6, 8, 6, 15},
                                            {161011, 16, 10, 13},
                                            {192, 17, 8, 14},
                                            {21037, 9, 7, 18, 13},
                                            {292, 11, 6, 16, 20} };
	
	std::cout << "\n";
	
	long result = sumOfTrueEquations2(list);
	
	if(result == 11387)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
