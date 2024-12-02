#pragma once

#include <vector>

class Day1
{
	public:

		bool run();

		bool calculateDistance_test();
		bool calculateSimilarityScore_test();		
	
	private:
	
		bool readInput(std::vector<int> * list_1, std::vector<int> * list_2);
	
		int calculateDistance(std::vector<int> list_1, std::vector<int> list_2);
		int calculateSimilarityScore(std::vector<int> list_1, std::vector<int> list_2);
};
