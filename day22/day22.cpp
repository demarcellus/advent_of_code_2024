#include "day22.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <map>

bool Day22::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<unsigned long> initial_secret_numbers;
	
	readInput(&initial_secret_numbers);
	
	// print
	std::cout << "INITIAL SECRET NUMBERS:\n";
	for(int i = 0; i < 10; i++)
	{
		std::cout << "[" << initial_secret_numbers[i] << "]\n";
	}
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	unsigned long result = getNthIterationSecretNumberSum(initial_secret_numbers, 2000);
	std::cout << ">>> 2000th ITERATION SECRET NUMBER SUM [" << result << "]\n";
	
	return true;
}

bool Day22::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
    if(ok) ok = operations_test();
	if(ok) ok = getNthIterationSecretNumberSum_test();

	return ok;
}

/**
 * CODE
 */
inline unsigned long mix(const unsigned long a, const unsigned long b)
{
	return a ^ b;
}

inline unsigned long prune(const unsigned long a)
{
	return a % 16777216UL;
}

inline unsigned long evolve(const unsigned long a)
{
	unsigned long b, c;
	
	b = (a << 6);
	b = mix(a, b);
	c = prune(b);
	
	b = (c >> 5);
	b = mix(b, c);
	c = prune(b);
	
	b = (c << 11);
	b = mix(b, c);
	c = prune(b);
	
	return c;
}
 
unsigned long Day22::getNthIterationSecretNumberSum(const std::vector<unsigned long>& initial_secret_numbers, const int& iterations)
{
	std::vector<unsigned long> output;
	std::map<unsigned long, unsigned long> cache;
	
	for(const auto& secret_number : initial_secret_numbers)
	{
		unsigned long s = secret_number;
		
		for(int i = 0; i < iterations; i++)
		{
			if(cache.count(s) == 0)
			{
				unsigned long out = evolve(s);
				
				cache[s] = out;
			}
			
			s = cache[s];			
		}		
		
		output.push_back(s);
	}
	
	unsigned long sum = 0;
	//std::cout << "result:\n";
	for(const auto& o : output)
	{
		//std::cout << "[" << o << "]\n";
		
		sum += o;
	}
	
	//std::cout << "sum [" << sum << "]\n";
	
	return sum;
}

/**
 * INPUT
 */
bool Day22::readInput(std::vector<unsigned long> * initial_secret_numbers)
{
	std::string line;
	
	std::ifstream infile("data/day22.txt");

	initial_secret_numbers->clear();

	while(getline(infile, line))
	{
		initial_secret_numbers->push_back(std::stol(line));
	}	
		
	return true;
}


/**
 * TESTS 
 */
bool Day22::operations_test()
{
	bool ok = true;
	
	
	if(ok)
	{
		unsigned long a = 42;
		unsigned long b = 15;
		
		unsigned long c = mix(a,b);
		
		std::cout << "> mix test [" << (c==37?"OK":"FAILED") << "]\n";
		
		if(c != 37) ok = false;		
	}
	
	if(ok)
	{
		unsigned long a = 100000000;
		
		unsigned long b = prune(a);
		
		std::cout << "> prune test [" << (b==16113920?"OK":"FAILED") << "]\n";
		
		if(b != 16113920) ok = false;
	}
	
	if(ok)
	{
		unsigned long s = 123;
		std::vector<unsigned long> res;
		std::vector<unsigned long> exp = {15887950, 16495136, 527345, 704524, 1553684, 12683156, 11100544, 12249484, 7753432, 5908254};
		
		for(int i = 0; i < 10; i++)
		{
			s = evolve(s);
			
			//std::cout << "[" << i << "] --> res[" << s << "] exp[" << exp[i] <<"]\n";
			
			res.push_back(s);
		}
		
		std::cout << "> evolve test [" << (res==exp?"OK":"FAILED") << "]\n";
		
		if(res != exp) ok = false;
		
	}
	
	
	if(ok)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	
	return false;
} 
 
 
bool Day22::getNthIterationSecretNumberSum_test()
{
	std::vector<unsigned long> initial_secret_numbers = { 1, 10, 100, 2024 };
	
	std::cout << "\n";
	
	int result = getNthIterationSecretNumberSum(initial_secret_numbers, 2000);
	
	if(result == 37327623)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
