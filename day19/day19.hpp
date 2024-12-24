#pragma once

#include <vector>
#include <string>

class Day19
{
	public:

		bool run();
		bool test();

	private:
	
	
		bool readInput(std::vector<std::string> * patterns, std::vector<std::string> * requested_designs);

		
		int numberOfPossibleDesigns(const std::vector<std::string>& patterns, const std::vector<std::string>& requested_designs);
		long long numberOfPossibleDesignArrangements(const std::vector<std::string>& patterns, const std::vector<std::string>& requested_designs);

		bool tryPatternsFromBehind(const std::vector<std::string>& patterns, const std::string& requested_design);
		long long tryPatterns(const std::vector<std::string>& patterns, const std::string& s, const int depth, const int max_string_length);

		bool numberOfPossibleDesigns_test();
		bool numberOfPossibleDesignArrangements_test();

		
};
