#pragma once

#include <vector>

class Day5
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<int>> * rules, std::vector<std::vector<int>> * pages_list);
		
		int countCorrectOrder(std::vector<std::vector<int>> rules, std::vector<std::vector<int>> pages_list);
		int reorderInCorrectAndCount(std::vector<std::vector<int>> rules, std::vector<std::vector<int>> pages_list);

		bool countCorrectOrder_test();
		bool reorderInCorrectAndCount_test();
		
		std::vector<std::vector<int>> filterRulesForPages(std::vector<std::vector<int>> rules, std::vector<int> pages);
		bool checkSorting(std::vector<std::vector<int>> rules, std::vector<int> pages);
		
		void printRules(std::vector<std::vector<int>> rules);
		void printPages(std::vector<int> pages);
};
