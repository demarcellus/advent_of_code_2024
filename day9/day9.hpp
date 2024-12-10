#pragma once

#include <vector>

class Day9
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<int> * diskmap);
		
		bool movefilesAndCalculateChecksum_test();
		bool movefilesNotFragmentedAndCalculateChecksum_test();
		
		
		long movefilesAndCalculateChecksum(std::vector<int> diskmap);
		long movefilesNotFragmentedAndCalculateChecksum(std::vector<int> diskmap);

		int fistFreeSpace(std::vector<std::vector<int>>& diskmap_list);
		int firstLargeEnoughFreeSpace(std::vector<std::vector<int>>& diskmap_list, int size);
		int lastFile(std::vector<std::vector<int>>& diskmap_list, int from);

		void printDiskmapList(std::vector<std::vector<int>> diskmap_list);
		void printDiskmapListFormatted(std::vector<std::vector<int>> diskmap_list);
};
