#pragma once

#include <vector>

class Day2
{
	public:

		bool run();
		bool test();



	private:
	
		bool readInput(std::vector<std::vector<int>> * v);
		
		int countSafeReports(std::vector<std::vector<int>> v);
		int countSafeReportsProblemDampener(std::vector<std::vector<int>> v);

		bool vectorIsSafe(std::vector<int> v);
		void printVector(std::vector<int> v);
		
		bool countSafeReports_test();
		bool countSafeReportsProblemDampener_test();
};
