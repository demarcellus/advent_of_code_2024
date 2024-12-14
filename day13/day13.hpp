#pragma once

#include <vector>

class Day13
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<std::pair<int,int>>> * machines);

		int optimizeMachines(std::vector<std::vector<std::pair<int,int>>> machines);
		long long optimizeMachines2(std::vector<std::vector<std::pair<int,int>>> machines, bool increment);

		bool optimizeMachines_test();
		bool optimizeMachines2_test();

};
