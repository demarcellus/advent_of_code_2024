#include "day2.hpp"
		
#include <iostream>

#include <sstream>
#include <fstream>
#include <string>

bool Day2::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<int>> v;
	
	readInput(&v);
	
	// print
	for(size_t i = 0; i < 5; i++)
	{
		std::cout << i << " [ ";
		for(size_t j = 0; j < v.at(i).size(); j++)
		{
			std::cout << v.at(i).at(j) << " ";
		}
		std::cout << "]\n";
	}

	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int safe_reports = countSafeReports(v);

	std::cout << ">>> SAFE REPORTS: [" << safe_reports << "]\n";

	int safe_reports_dampener = countSafeReportsProblemDampener(v);
	
	std::cout << ">>> SAFE REPORTS DAMPENER: [" << safe_reports_dampener << "]\n";
	
	return true;
}

bool Day2::test()
{
	bool ok = true;
	
	std::cout << "TESTS:\n";
	
	if(ok) ok = countSafeReports_test();
	
	if(ok) ok = countSafeReportsProblemDampener_test();
	
	return ok;
}

int Day2::countSafeReports(std::vector<std::vector<int>> v)
{
	int count = 0;
	
	for(size_t i = 0; i < v.size(); i++)
	{		
		if(vectorIsSafe(v[i]))
		{
			count++;
		}
	}
	
	return count;
}

int Day2::countSafeReportsProblemDampener(std::vector<std::vector<int>> v)
{
	int count = 0;
	
	for(size_t i = 0; i < v.size(); i++)
	{
		bool safe = true;
		
		// copy vector and check
		std::vector<int> w = v[i];
		safe = vectorIsSafe(w);
		
		// if unsafe, use "dampener" to remove one value and check again
		if(!safe)
		{
			safe = true;
			
			for(size_t r = 0; r < w.size(); r++)
			{
				// copy, remove one item and check
				std::vector<int> q = w;
				q.erase(q.begin() + r);
				safe = vectorIsSafe(q);
				
				// only one has to be safe				
				if(safe) break;
			}
		}
		
		if(safe)
		{
			count++;
		}
	}
	
	return count;
}

bool Day2::vectorIsSafe(std::vector<int> v)
{
	bool safe = true;
		
	int val_prev = v[0];
	int val;
	int delta;
	int sign_prev = 0;
	int sign;
		
	for(size_t j = 1; j < v.size(); j++)
	{			
		val = v[j];
		delta = val - val_prev;
		sign = delta / abs(delta);
			
		// check sign
		if(sign_prev == 0) sign_prev = sign; // first
		if(sign != sign_prev){ safe = false; break; }
			
		// check delta
		if(delta == 0 || abs(delta) > 3){ safe = false; break; }
			
		val_prev = val;
		sign_prev = sign;
	}
	
	return safe;	
}

void Day2::printVector(std::vector<int> v)
{
	std::cout << "[";
	
	for(size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << " ";
	}
	
	std::cout << "]";
}

/**
 * INPUT
 */
bool Day2::readInput(std::vector<std::vector<int>> * v)
{
	std::ifstream infile("data/day2.txt");
	
	v->clear();
	std::string line;
	
	while(getline(infile, line))
	{
		std::stringstream ss(line);
		std::string tok;
		int val;
		std::vector<int> w;
		
		while(getline(ss, tok, ' '))
		{
			val = stoi(tok);
			w.push_back(val);
		}		
		
		v->push_back(w);
	}	
		
	return true;
}

/**
 * TESTS 
 */
bool Day2::countSafeReports_test()
{
	
	std::vector<std::vector<int>> v = { {7, 6, 4, 2, 1},
		                                {1, 2, 7, 8, 9},
		                                {9, 7, 6, 2, 1},
		                                {1, 3, 2, 4, 5},
		                                {8, 6, 4, 4, 1},
		                                {1, 3, 6, 7, 9} };
		                      
	int safe_reports = countSafeReports(v);
	
	if(safe_reports == 2)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	return false;
}

bool Day2::countSafeReportsProblemDampener_test()
{
	
	std::vector<std::vector<int>> v = { {7, 6, 4, 2, 1},
		                                {1, 2, 7, 8, 9},
		                                {9, 7, 6, 2, 1},
		                                {1, 3, 2, 4, 5},
		                                {8, 6, 4, 4, 1},
		                                {1, 3, 6, 7, 9} };
		                      
	int safe_reports = countSafeReportsProblemDampener(v);
	
	if(safe_reports == 4)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	return false;
}
