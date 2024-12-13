#include "day11.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <algorithm>

#include <map>

bool Day11::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<long> arrangement;
	
	readInput(&arrangement);
	
	// print
	std::cout << "[";
	for(auto item : arrangement)
	{
		std::cout << item << " ";
	}
	std::cout << "]\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	long result = countStonesAfterBlinkingRecursive(arrangement, 25);
	std::cout << ">>> NUMBER_OF_STONES (25): [" << result << "]\n";
	
	long result2 = countStonesAfterBlinkingRecursive(arrangement, 75);
	std::cout << ">>> NUMBER_OF_STONES (75): [" << result2 << "]\n";
	
				
	return true;
}

bool Day11::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = countStonesAfterBlinking_test();
	if(ok) ok = countStonesAfterBlinkingRecursive_test();
	
	return ok;
}

/**
 * CODE
 */
 
// yeah this kills itself after 44 iterations :-)
long Day11::countStonesAfterBlinking(std::vector<long> arrangement, int iterations)
{
	for(int i = 0; i < iterations; i++)
	{
		std::cout << "blink iteration [" << (i + 1) << "/" << iterations << "]\n";
		blink(arrangement);
	}
	
	return arrangement.size();
}


long Day11::countStonesAfterBlinkingRecursive(std::vector<long> arrangement, int iterations)
{
	long sum = 0;
	
	for(auto stone : arrangement)
	{
		sum += blinkRecursive(stone, iterations);
	}
	
	return sum;
}

long Day11::blinkRecursive(long stone, int iterations_left)
{
	static std::map<std::pair<long, int>, long> known_stones;
	
	if(iterations_left == 0) return 1;
	
	// check if we already know this number + iterations combo
	std::pair<long, int> index = {stone, iterations_left};
	if(known_stones.count(index))
	{
		return known_stones[index];
	}
	
	// do blink stuff
	long count = 0;
	
	if(stone == 0)
	{
		count += blinkRecursive(1, iterations_left - 1);
	}
	else if(hasEvenNumberOfDigits(stone))
	{
		std::vector<long> v = splitNumber(stone);
		
		count += blinkRecursive(v[0], iterations_left - 1);
		count += blinkRecursive(v[1], iterations_left - 1);
	}
	else
	{
		count += blinkRecursive(stone * 2024, iterations_left - 1);
	}
	
	// save combo
	known_stones[index] = count;
	
	return count;
}

void Day11::blink(std::vector<long>& arrangement)
{
	std::vector<long> arrangement_new;
		
	for(int i = 0; i < (int)arrangement.size(); i++)
	{
		long stone = arrangement[i];
		
		if(stone == 0)
		{
			arrangement_new.push_back(1);
		}
		else if(hasEvenNumberOfDigits(stone))
		{
			std::vector<long> v = splitNumber(stone);
			arrangement_new.push_back(v[0]);
			arrangement_new.push_back(v[1]);
		}
		else
		{
			long new_stone = 2024 * stone;
			arrangement_new.push_back(new_stone);
		}
	}
	
	arrangement = arrangement_new;
}

bool Day11::hasEvenNumberOfDigits(long x)
{
	std::string a = std::to_string(x);
	
	return (a.size() % 2) == 0;
}

std::vector<long> Day11::splitNumber(long x)
{
	std::string a = std::to_string(x);
	long len = a.size();
	
	std::vector<long> v;
	
	v.push_back(std::stol(a.substr(0,len/2)));
	v.push_back(std::stol(a.substr(len/2,len/2)));
	
	return v;
}

/**
 * INPUT
 */
bool Day11::readInput(std::vector<long> * arrangement)
{
	std::string line;
	
	// load rules
	std::ifstream infile("data/day11.txt");

	arrangement->clear();

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
		
		*arrangement = w;
		return true;
	}
	
	return true;
}

/**
 * TESTS 
 */
bool Day11::countStonesAfterBlinking_test()
{
	std::vector<long> arrangement = { 125, 17 };
	
	std::cout << "\n";
	
	long result = countStonesAfterBlinking(arrangement, 25);
	
	if(result == 55312)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day11::countStonesAfterBlinkingRecursive_test()
{
	std::vector<long> arrangement = { 125, 17 };
	
	std::cout << "\n";
	
	long result = countStonesAfterBlinkingRecursive(arrangement, 25);
	
	if(result == 55312)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
