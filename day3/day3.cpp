#include "day3.hpp"
		
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <iterator>


bool Day3::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::string s;
	
	readInput(&s);
	
	// print
	std::cout << "[ ";
	for(size_t i = 0; i < 64; i++)
	{
		std::cout << s[i];
	}
	std::cout << " ]\n";

	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = filterUncorruptedMultiplyAndAdd(s);

	std::cout << ">>> FILTERED MULT. RESULT: [" << result << "]\n";

	int result_do = filterUncorruptedDoDontMultiplyAndAdd(s);
	
	std::cout << ">>> FILTERED MULT. DO/DON'T RESULT: [" << result_do << "]\n";
	
	
	return true;
}

bool Day3::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = filterUncorruptedMultiplyAndAdd_test();
	if(ok) ok = filterUncorruptedDoDontMultiplyAndAdd_test();
	
	return ok;
}

int Day3::filterUncorruptedMultiplyAndAdd(std::string s)
{	
	int sum = 0;
	
	std::regex r("mul\\(\\d+,\\d+\\)");
	
	auto mult_begin = std::sregex_iterator(s.begin(), s.end(), r);
	auto mult_end = std::sregex_iterator();
	
	//std::cout << "Found [" << std::distance(mult_begin, mult_end) << "] matches\n";
	
	for(std::sregex_iterator i = mult_begin; i != mult_end; ++i)
	{
		auto match = *i;
		std::string match_str = match.str();
		
		//std::cout << ": [" << match_str << "] --> ";
		
		int a,b;
		
		int num = sscanf(match_str.c_str(), "mul(%d,%d)", &a, &b);
		
		if(num != 2)
		{
			std::cout << "PARSE ERROR!\n";
			return -1;
		}
		
		int res = a * b;
		
		//std::cout << "result [" << res << "]\n";
		
		sum += res;
		
	}
	
	return sum;
}

int Day3::filterUncorruptedDoDontMultiplyAndAdd(std::string s)
{
	int sum = 0;
	bool enabled = true;
	
	std::regex r("mul\\(\\d+,\\d+\\)|(do\\(\\))|(don't\\(\\))");
	
	auto mult_begin = std::sregex_iterator(s.begin(), s.end(), r);
	auto mult_end = std::sregex_iterator();
	
	//std::cout << "Found [" << std::distance(mult_begin, mult_end) << "] matches\n";
	
	for(std::sregex_iterator i = mult_begin; i != mult_end; ++i)
	{
		auto match = *i;
		std::string match_str = match.str();
		
		//std::cout << ": [" << match_str << "] --> ";
		
		int res;
		
		if(match_str.find("don't()") != std::string::npos)
		{
			//std::cout << "[DONT] --> ";
			enabled = false;
		}
		else if(match_str.find("do()") != std::string::npos)
		{
			//std::cout << "[DO] --> ";
			enabled = true;
		}
		else if(match_str.find("mul(") != std::string::npos)
		{
			//std::cout << "[MUL] --> ";
			
			int a,b;
			
			int num = sscanf(match_str.c_str(), "mul(%d,%d)", &a, &b);
		
			if(num != 2)
			{
				std::cout << "PARSE ERROR!\n";
				return -1;
			}
			
			res = a * b;
			
			//std::cout << "result [" << res << "] --> add [" << (enabled?"YES":"NO") << "] --> ";
			
			if(enabled)
			{
				sum += res;
			}
			
			//std::cout << "sum [" << sum << "]";
		}
		else
		{
			std::cout << "UNKNOWN INSTRUCTION!\n";
			return -1;
		}
		
		//std::cout << "\n";
		
	}
	
	return sum;
}

/**
 * INPUT
 */
bool Day3::readInput(std::string * s)
{
	std::ifstream infile("data/day3.txt");
	
	std::getline(infile, *s, std::string::traits_type::to_char_type(std::string::traits_type::eof()));
		
	return true;
}

/**
 * TESTS 
 */
bool Day3::filterUncorruptedMultiplyAndAdd_test()
{
	std::string s = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
	
	int result = filterUncorruptedMultiplyAndAdd(s);
	
	if(result == 161)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day3::filterUncorruptedDoDontMultiplyAndAdd_test()
{
	std::string s = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
	
	int result = filterUncorruptedDoDontMultiplyAndAdd(s);
	
	if(result == 48)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

