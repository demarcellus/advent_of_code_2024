#include "day9.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <cctype>

#include <algorithm>

bool Day9::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<int> diskmap;
	
	readInput(&diskmap);
	
	// print
	std::cout << "[";
	for(size_t i = 0; i < 32; i++)
	{
		std::cout << diskmap[i];
	}
	std::cout << " ...]\n";

	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	long result = movefilesAndCalculateChecksum(diskmap);
	std::cout << ">>> CHECKSUM: [" << result << "]\n";
		
	long result2 = movefilesNotFragmentedAndCalculateChecksum(diskmap);
	std::cout << ">>> UNFRAG. CHECKSUM: [" << result2 << "]\n";
		
	return true;
}

bool Day9::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";
    
	if(ok) ok = movefilesAndCalculateChecksum_test();
	if(ok) ok = movefilesNotFragmentedAndCalculateChecksum_test();
	
	return ok;
}

/**
 * CODE
 */
long Day9::movefilesAndCalculateChecksum(std::vector<int> diskmap)
{
	
	// make list of diskmap
	std::vector<std::vector<int>> diskmap_list;	// list of {id, length} where id = -1 = free space
	bool is_file = true;
	int id = 0;
	for(auto entry : diskmap)
	{		
		diskmap_list.push_back(std::vector<int> {(is_file?id++:-1), entry});
		is_file = !is_file;
	}

	// loop through list
	bool done = false;
	while(true)
	{
		// last block info
		std::vector<int> item = diskmap_list.back();
		int last_file_id = item[0];
		int last_file_size = item[1];
		
		if(last_file_id == -1)
		{
			diskmap_list.pop_back();
			continue;
		}
		
		do
		{
			// find first free space
			int index_free = fistFreeSpace(diskmap_list);
			
			if(index_free == -1)
			{
				done = true;
				break;
			}
			
			// get available space
			int free_space = diskmap_list[index_free][1];

			// copy data
			if(last_file_size > free_space)
			{
				// too big for free space
				
				// --> copy a part of the last block
				diskmap_list[index_free] = std::vector<int> {last_file_id, free_space};
				last_file_size -= free_space;
				
			}
			else if(last_file_size == free_space)
			{	
				// equal to free space -- easy one
				diskmap_list[index_free] = std::vector<int> {last_file_id, last_file_size};
				last_file_size = 0;
			}
			else
			{
				// smaller than free space
				
				int new_free_space = free_space - last_file_size;
				
				// --> copy first
				diskmap_list[index_free] = std::vector<int> {last_file_id, last_file_size};
				last_file_size = 0;
				
				// --> insert "new" free space in list				
				diskmap_list.insert(diskmap_list.begin() + index_free + 1, std::vector<int> {-1, new_free_space});
			}			
		}
		while(last_file_size > 0);
		
		if(done)
		{
			// update last block
			diskmap_list.back() = std::vector<int> {last_file_id, last_file_size};
			
			// combine last blocks?
			if((diskmap_list.end() - 1)->at(0) == (diskmap_list.end() - 2)->at(0))
			{
				int file_size = (diskmap_list.end() - 1)->at(1) + (diskmap_list.end() - 2)->at(1);
				(diskmap_list.end() - 2)->at(1) = file_size;
				
				diskmap_list.pop_back();
			}				
			
			break;
		}
				
		// remove last block if zero		
		diskmap_list.pop_back();
	}
	
	// make "checksum"
	long checksum = 0;
	int index = 0;
	for(auto item : diskmap_list)
	{
		for(int i = 0; i < item[1]; i++)
		{
			if(item[0] > 0)	checksum += (item[0] * index);
			index++;
		}
	}
	
	return checksum;
}

long Day9::movefilesNotFragmentedAndCalculateChecksum(std::vector<int> diskmap)
{
	// make list of diskmap
	std::vector<std::vector<int>> diskmap_list;	// list of {id, length} where id = -1 = free space
	bool is_file = true;
	int id = 0;
	for(auto entry : diskmap)
	{		
		diskmap_list.push_back(std::vector<int> {(is_file?id++:-1), entry});
		is_file = !is_file;
	}

	//printDiskmapListFormatted(diskmap_list);

	// loop through list
	int index_last = -1; // start indication
	do
	{
		
		// find last file id
		index_last = lastFile(diskmap_list, index_last);
				
		if(index_last == -1) 
		{
			//std::cout << "no last file found\n";
			break;
		}
		
		int last_file_id = diskmap_list[index_last][0];
		int last_file_size = diskmap_list[index_last][1];
		
		//std::cout << "last file - index [" << index_last << "] id [" << last_file_id << "] size [" << last_file_size << "]\n";
		
		// find first fitting empty space
		int index_free_fitting = firstLargeEnoughFreeSpace(diskmap_list, last_file_size);
		
		if(index_free_fitting == -1)
		{
			//std::cout << "no fitting space for file [" << last_file_id << "] size [" << last_file_size << "]\n";
			index_last--; // move search one closer to start
			continue;
		}
		if(index_free_fitting >= index_last)
		{
			//std::cout << "index crossing\n";
			index_last--; //move search one closer to start
			continue;
		}
		
		int free_fitting_size = diskmap_list[index_free_fitting][1];
		int free_fitting_size_new = free_fitting_size - last_file_size;
		
		//std::cout << "fitting space found - index [" << index_free_fitting << "] size [" << free_fitting_size << "]\n";

		// move stuff		
		diskmap_list[index_last][0] = -1;
		if(free_fitting_size_new > 0)
		{
			diskmap_list[index_free_fitting][1] = free_fitting_size_new;
			diskmap_list.insert(diskmap_list.begin() + index_free_fitting, std::vector<int> {last_file_id, last_file_size});	
		}
		else
		{
			diskmap_list[index_free_fitting][0] = last_file_id;
		}	
		
		//printDiskmapListFormatted(diskmap_list);
	}
	while(index_last > 0);
	
	//printDiskmapListFormatted(diskmap_list);
	//printDiskmapList(diskmap_list);
	
	// make "checksum"
	long checksum = 0;
	int index = 0;
	for(auto item : diskmap_list)
	{
		for(int i = 0; i < item[1]; i++)
		{
			if(item[0] > 0)	checksum += (item[0] * index);
			index++;
		}
	}
	
	//std::cout << "checksum [" << checksum << "]\n";
	
	return checksum;
}

int Day9::fistFreeSpace(std::vector<std::vector<int>>& diskmap_list)
{
	for(int i = 0; i < (int)diskmap_list.size(); i++)
	{
		if(diskmap_list[i][0] == -1) return i;
	}
	
	return -1;
}
int Day9::firstLargeEnoughFreeSpace(std::vector<std::vector<int>>& diskmap_list, int size)
{
	for(int i = 0; i < (int)diskmap_list.size(); i++)
	{
		if(diskmap_list[i][0] == -1 && diskmap_list[i][1] >= size) return i;
	}
	
	return -1;
}

int Day9::lastFile(std::vector<std::vector<int>>& diskmap_list, int from)
{
	if(from == -1) from = (int)diskmap_list.size() - 1;
	
	for(int i = from; i >= 0; i--)
	{
		if(diskmap_list[i][0] > -1) return i;
	}
	
	return -1;
}

void Day9::printDiskmapList(std::vector<std::vector<int>> diskmap_list)
{
	for(auto item : diskmap_list)
	{
		std::cout << "[" << item[0] << "," << item[1] << "]\n";
	}
}

void Day9::printDiskmapListFormatted(std::vector<std::vector<int>> diskmap_list)
{
	for(auto item : diskmap_list)
	{
		for(int i = 0; i < item[1]; i++)
		{
			std::cout << (item[0]==-1?".":(std::to_string(item[0])));
		}
		
	}
	std::cout << "\n";
}

/**
 * INPUT
 */
bool Day9::readInput(std::vector<int> * diskmap)
{
	std::string line;
	
	// load rules
	std::ifstream infile("data/day9.txt");

	diskmap->clear();

	while(getline(infile, line))
	{
		for(int i = 0; i < (int)line.size(); i++)
		{
			int val = (line[i] - '0');
			diskmap->push_back(val);
		}
	}	
	
	return true;
}

/**
 * TESTS 
 */
bool Day9::movefilesAndCalculateChecksum_test()
{
	std::vector<int> diskmap = { 2,3,3,3,1,3,3,1,2,1,4,1,4,1,3,1,4,0,2 };
	
	std::cout << "\n";
	
	long result = movefilesAndCalculateChecksum(diskmap);
	
	if(result == 1928)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day9::movefilesNotFragmentedAndCalculateChecksum_test()
{
	std::vector<int> diskmap = { 2,3,3,3,1,3,3,1,2,1,4,1,4,1,3,1,4,0,2 };
	
	std::cout << "\n";
	
	long result = movefilesNotFragmentedAndCalculateChecksum(diskmap);
	
	if(result == 2858)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
