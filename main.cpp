#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

/**
 * FUNCTION PROTOTYPES
 */
int day1_calculateDistance(std::vector<int> list_1, std::vector<int> list_2);
int day1_calculateSimilarityScore(std::vector<int> list_1, std::vector<int> list_2);

bool day1_readInputLists(std::vector<int> * list_1, std::vector<int> * list_2);

bool day1_calculateDistance_test();
bool day1_calculateSimilarityScore_test();

/**
 * MAIN
 */
int main()
{
	bool ok = true;
	
	// RUN TESTS
	std::cout << "TESTS:\n";
	if(ok) ok = day1_calculateDistance_test();
	if(ok) ok = day1_calculateSimilarityScore_test();

	if(!ok) return -1;

	// GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<int> v1, v2;
	
	day1_readInputLists(&v1, &v2);
	
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
	
	int distance = day1_calculateDistance(v1, v2);

	std::cout << ">>> DISTANCE: [" << distance << "]\n";

	int similarity_score = day1_calculateSimilarityScore(v1, v2);
	
	std::cout << ">>> SIMILARITY SCORE: [" << similarity_score << "]\n";

	return 0;
}

/**
 * FUNCTIONS
 */
int day1_calculateDistance(std::vector<int> list_1, std::vector<int> list_2)
{
	std::sort(list_1.begin(), list_1.end());
	std::sort(list_2.begin(), list_2.end());
	
	int sum = 0;
	
	for(size_t i = 0; i < list_1.size(); i++)
	{
		int dist = abs(list_1[i]-list_2[i]);
		
		//std::cout << i << ": [" << list_1[i] << "] [" << list_2[i] << "] [" << dist << "]\n";
		
		sum += dist;
	}	
				
	return sum;
}

int day1_calculateSimilarityScore(std::vector<int> list_1, std::vector<int> list_2)
{
	std::map<int, int> occurances_map;
	
	int sum = 0;
	
	for(size_t i = 0; i < list_1.size(); i++)
	{
		int number = list_1[i];
		int occurances;
		int similarity_score;
		
		//std::cout << "[" << number << "] --> ";
		
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
			
		//std::cout << "occurances [" << occurances << "] --> ";
		
		// make score
		similarity_score = number * occurances;
		
		//std::cout << "similarity_score [" << similarity_score << "]\n";
		
		// make sum
		sum += similarity_score;
	}
	
	//std::cout << "sum [" << sum << "]\n";
	
	return sum;
}

/**
 * INPUT
 */
bool day1_readInputLists(std::vector<int> * list_1, std::vector<int> * list_2)
{
	std::ifstream infile("data/day1_lists.txt");
	
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
bool day1_calculateDistance_test()
{
	std::vector<int> v1 = {3, 4, 2, 1, 3, 3};
	std::vector<int> v2 = {4, 3, 5, 3, 9, 3};
	
	int output = day1_calculateDistance(v1, v2);
	
	if(output == 11)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	return false;
}

bool day1_calculateSimilarityScore_test()
{
	std::vector<int> v1 = {3, 4, 2, 1, 3, 3};
	std::vector<int> v2 = {4, 3, 5, 3, 9, 3};
	
	int output = day1_calculateSimilarityScore(v1, v2);
	
	if(output == 31)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";
	return false;
}
