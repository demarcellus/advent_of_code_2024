#include "day1.hpp"

#include <iostream>

#include <map>
#include <algorithm>
#include <fstream>

int Day1::calculateDistance(std::vector<int> list_1, std::vector<int> list_2)
{
	std::sort(list_1.begin(), list_1.end());
	std::sort(list_2.begin(), list_2.end());
	
	int sum = 0;
	
	for(size_t i = 0; i < list_1.size(); i++)
	{
		int dist = abs(list_1[i]-list_2[i]);
		
		sum += dist;
	}	
				
	return sum;
}

int Day1::calculateSimilarityScore(std::vector<int> list_1, std::vector<int> list_2)
{
	std::map<int, int> occurances_map;
	
	int sum = 0;
	
	for(size_t i = 0; i < list_1.size(); i++)
	{
		int number = list_1[i];
		int occurances;
		int similarity_score;
		
		// check if "key" exists
		if(occurances_map.find(number) == occurances_map.end())
		{
			// find occurances for new "key"
			occurances = std::count(list_2.begin(), list_2.end(), number);
			
			// place in map
			occurances_map[number] = occurances;		
		}		
			
		// get occurances
		occurances = occurances_map[number];

		// make score
		similarity_score = number * occurances;
		
		// sum
		sum += similarity_score;
	}
		
	return sum;
}

bool Day1::run()
{
	// GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<int> v1, v2;
	
	readInput(&v1, &v2);
	
	// print
	for(size_t i = 0; i < 5; i++)
	{
		std::cout << i << ": [" << v1[i] << "][" << v2[i] << "]\n";
	} 
	std::cout << "...\n";
	std::cout << "total size [" << v1.size() << "]\n";

	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int distance = calculateDistance(v1, v2);

	std::cout << ">>> DISTANCE: [" << distance << "]\n";

	int similarity_score = calculateSimilarityScore(v1, v2);
	
	std::cout << ">>> SIMILARITY SCORE: [" << similarity_score << "]\n";
	
	return true;
}

/**
 * INPUT
 */
bool Day1::readInput(std::vector<int> * list_1, std::vector<int> * list_2)
{
	std::ifstream infile("data/day1.txt");
	
	list_1->clear();
	list_2->clear();
	
	int a, b;
	while(infile >> a >> b)
	{
		list_1->push_back(a);
		list_2->push_back(b);
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day1::calculateDistance_test()
{
	std::vector<int> v1 = {3, 4, 2, 1, 3, 3};
	std::vector<int> v2 = {4, 3, 5, 3, 9, 3};
	
	int output = calculateDistance(v1, v2);
	
	if(output == 11)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	return false;
}

bool Day1::calculateSimilarityScore_test()
{
	std::vector<int> v1 = {3, 4, 2, 1, 3, 3};
	std::vector<int> v2 = {4, 3, 5, 3, 9, 3};
	
	int output = calculateSimilarityScore(v1, v2);
	
	if(output == 31)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	return false;
}
