#pragma once

#include <vector>
#include <map>
#include <string>

class Day11
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<long> * arrangement);
	
		long countStonesAfterBlinking(std::vector<long> arrangement, int iterations);
		long countStonesAfterBlinkingRecursive(std::vector<long> arrangement, int iterations);

		void blink(std::vector<long>& arrangement);
		long blinkRecursive(long stone, int iterations_left);
		
		bool hasEvenNumberOfDigits(long x);
		std::vector<long> splitNumber(long x);
		
		bool countStonesAfterBlinking_test();
		bool countStonesAfterBlinkingRecursive_test();

};
