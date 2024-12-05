#include "day5.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>

bool Day5::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::vector<int>> rules;
	std::vector<std::vector<int>> pages_list;
	
	readInput(&rules, &pages_list);
	
	// print
	std::cout << "RULES:\n";
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[" << rules[i][0] << "|" << rules[i][1] << "]\n";
	}
	
	// print
	std::cout << "PAGES:\n";
	for(size_t i = 0; i < 16; i++)
	{
		std::cout << "[" << i << "] ";
		printPages(pages_list[i]);
		std::cout << "\n";
	}

	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = countCorrectOrder(rules, pages_list);

	std::cout << ">>> FOUND CORRECT ORDERED MIDDLE SUM: [" << result << "]\n";
	
	int result2 = reorderInCorrectAndCount(rules, pages_list);

	std::cout << ">>> REORDERED MIDDLE SUM: [" << result2 << "]\n";
	
	return true;
}

bool Day5::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = countCorrectOrder_test();
	if(ok) ok = reorderInCorrectAndCount_test();
	
	return ok;
}

int Day5::countCorrectOrder(std::vector<std::vector<int>> rules, std::vector<std::vector<int>> pages_list)
{
	int count = 0;

	for(size_t pages_list_index = 0; pages_list_index < pages_list.size(); pages_list_index++)
	{
		bool ok = true;
		
		std::vector<int> pages = pages_list[pages_list_index];
		
		std::cout << "[" << pages_list_index << "] PAGES: ";
		printPages(pages);
		std::cout << " --> ";
		
		// make sure we limit our rules for a set of pages
		std::vector<std::vector<int>> filtered_rules = filterRulesForPages(rules, pages);

		ok = checkSorting(filtered_rules, pages);
		
		if(ok)
		{
			std::cout << "ORDER OK --> ";
			
			size_t middle_index = (pages.size() - 1)/2;
			int    middle_value = pages[middle_index];
			
			count += middle_value;
			
			std::cout << "VALUE [" << middle_value << "] --> COUNT [" << count << "]";
		}
		else
		{
			std::cout << "FAILED";
		}
		
		std::cout << "\n";		
	}
	
	return count;
}

int Day5::reorderInCorrectAndCount(std::vector<std::vector<int>> rules, std::vector<std::vector<int>> pages_list)
{
	int count = 0;

	for(size_t pages_list_index = 0; pages_list_index < pages_list.size(); pages_list_index++)
	{
		bool ok = true;
		
		std::vector<int> pages = pages_list[pages_list_index];
		
		std::cout << "[" << pages_list_index << "] PAGES: ";
		printPages(pages);
		std::cout << " --> ";
		
		// make sure we limit our rules for a set of pages
		std::vector<std::vector<int>> filtered_rules = filterRulesForPages(rules, pages);
		
		//std::cout << "\nCURRENT RULES:";
		//printRules(filtered_rules);
		
		ok = checkSorting(filtered_rules, pages);
		
		if(ok)
		{
			std::cout << "ORDER OK --> DO NOTHING";
		}
		else
		{
			std::cout << "ORDER WRONG --> REORDER:\n";
			
			std::vector<int> pages_reordered;
			
			pages_reordered.push_back(pages[0]);
			
			for(size_t i = 1; i < pages.size(); i++)
			{
				int page = pages[i];
				
				//std::cout << "current pages ";
				//printPages(pages_reordered);
				//std::cout << "\n";
				
				//std::cout << "page [" << page << "]\n";
				
				std::vector<int> pages_reordered_try = pages_reordered;
								
				// just try all insert positions and check ok
				int max = (int)pages_reordered_try.size() + 1;
				for(int pos = 0; pos < max; pos++)
				{
					//td::cout << "pos: [" << pos << "]\n";
					
					pages_reordered_try.insert(pages_reordered_try.begin() + pos, page);
					
					//std::cout << "current pages ";
					//printPages(pages_reordered_try);
					//std::cout << "\n";
					
					// apply filters again
					std::vector<std::vector<int>> filtered_rules_reordered_try = filterRulesForPages(filtered_rules, pages_reordered_try);
					
					if(checkSorting(filtered_rules_reordered_try, pages_reordered_try))
					{
						//std::cout << "ordering ok\n";
						pages_reordered = pages_reordered_try;
						break;
					}
					else
					{
						//std::cout << "ordering failed\n";
						pages_reordered_try = pages_reordered;
					}
				}
			}
			
			std::cout << ">>> NEW ORDER: ";
			printPages(pages_reordered);
			
			std::cout << " --> ORDER [" << (checkSorting(filtered_rules, pages_reordered)?"OK":"FAILED") << "] --> ";
			
			size_t middle_index = (pages_reordered.size() - 1)/2;
			int    middle_value = pages_reordered[middle_index];
			
			count += middle_value;
			
			std::cout << "VALUE [" << middle_value << "] --> COUNT [" << count << "]";
		}
		
		std::cout << "\n";		
	}
	
	return count;
}

std::vector<std::vector<int>> Day5::filterRulesForPages(std::vector<std::vector<int>> rules, std::vector<int> pages)
{
	std::vector<std::vector<int>> out;
	
	for(size_t i = 0; i < rules.size(); i++)
	{
		if( std::find(pages.begin(), pages.end(), rules[i][0]) != pages.end() &&
		    std::find(pages.begin(), pages.end(), rules[i][1]) != pages.end() )
		{
			out.push_back(rules[i]);
		}
	}
	
	return out;
}

bool Day5::checkSorting(std::vector<std::vector<int>> rules, std::vector<int> pages)
{
	for(size_t rules_index = 0; rules_index < rules.size(); rules_index++)
	{
		std::vector<int> rule = rules[rules_index];
			
		// find rule page in pages
		auto before_index = std::find(pages.begin(), pages.end(), rule[0]);
		auto after_index = std::find(pages.begin(), pages.end(), rule[1]);
			
		// check sorting order - one wrong sorting = done
		if(before_index >= after_index) 
		{
			return false;
		}
	}
	
	return true;
}

void Day5::printRules(std::vector<std::vector<int>> rules)
{
	for(size_t i = 0; i < rules.size(); i++)
	{
		std::cout << "[" << rules[i][0] << "|" << rules[i][1] << "]\n";
	}
}

void Day5::printPages(std::vector<int> pages)
{
	std::cout << "[";
	for(size_t i = 0; i < pages.size(); i++)
	{
		std::cout << pages[i] << (i < pages.size()-1?",":"");
	}
	std::cout << "]";

}

bool Day5::readInput(std::vector<std::vector<int>> * rules, std::vector<std::vector<int>> * pages_list)
{
	std::string line;
	
	// load rules
	std::ifstream infile_rules("data/day5_rules.txt");

	rules->clear();

	while(getline(infile_rules, line))
	{
		std::stringstream ss(line);
		std::string tok;
		int val;
		std::vector<int> rule;
		
		while(getline(ss, tok, '|'))
		{
			val = stoi(tok);
			rule.push_back(val);
		}		
		
		rules->push_back(rule);
	}
	
	// load updates	
	std::ifstream infile_pages_list("data/day5_pages_list.txt");
	
	pages_list->clear();

	while(getline(infile_pages_list, line))
	{
		std::stringstream ss(line);
		std::string tok;
		int val;
		std::vector<int> pages;
		
		while(getline(ss, tok, ','))
		{
			val = stoi(tok);
			pages.push_back(val);
		}		
		
		pages_list->push_back(pages);
	}
	
	return true;
}

/**
 * TESTS 
 */
bool Day5::countCorrectOrder_test()
{
	std::vector<std::vector<int>> rules = { {47, 53},		
                                            {97, 13},
                                            {97, 61},
                                            {97, 47},
                                            {75, 29},
                                            {61, 13},
                                            {75, 53},
                                            {29, 13},
                                            {97, 29},
                                            {53, 29},
                                            {61, 53},
                                            {97, 53},
                                            {61, 29},
                                            {47, 13},
                                            {75, 47},
                                            {97, 75},
                                            {47, 61},
                                            {75, 61},
                                            {47, 29},
                                            {75, 13},
                                            {53, 13} };
                                         
    std::vector<std::vector<int>> pages = { {75, 47, 61, 53, 29},
                                            {97, 61, 53, 29, 13},
                                            {75, 29, 13},
                                            {75, 97, 47, 61, 53},
                                            {61, 13, 29},
                                            {97, 13, 75, 29, 47} };
	
	std::cout << "\n";
	
	int result = countCorrectOrder(rules, pages);
	
	if(result == 143)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day5::reorderInCorrectAndCount_test()
{
	std::vector<std::vector<int>> rules = { {47, 53},		
                                            {97, 13},
                                            {97, 61},
                                            {97, 47},
                                            {75, 29},
                                            {61, 13},
                                            {75, 53},
                                            {29, 13},
                                            {97, 29},
                                            {53, 29},
                                            {61, 53},
                                            {97, 53},
                                            {61, 29},
                                            {47, 13},
                                            {75, 47},
                                            {97, 75},
                                            {47, 61},
                                            {75, 61},
                                            {47, 29},
                                            {75, 13},
                                            {53, 13} };
                                         
    std::vector<std::vector<int>> pages = { {75, 47, 61, 53, 29},
                                            {97, 61, 53, 29, 13},
                                            {75, 29, 13},
                                            {75, 97, 47, 61, 53},
                                            {61, 13, 29},
                                            {97, 13, 75, 29, 47} };
	
	std::cout << "\n";
	
	int result = reorderInCorrectAndCount(rules, pages);
	
	if(result == 123)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

