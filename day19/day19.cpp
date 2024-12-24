#include "day19.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <algorithm>

#include <queue>
#include <climits>

#include <unordered_map>

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static bool ends_with(std::string_view str, std::string_view suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0;
}

static void remove_last(std::string &str, int n) 
{ 
    if ((int)str.size() < n) return;

    str.erase(str.size() - n);
}

struct Comparator
{
	bool operator()(const std::string& s1, const std::string& s2) const 
	{
		return s1.size() > s2.size(); 
	}
};

static int max_string_length(const std::vector<std::string>& v)
{
	int max_length = INT_MIN;
	
	for(auto& item : v)
	{
		int length = item.size();
		if(length > max_length) max_length = length;
	}
	
	return max_length;
}

bool Day19::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<std::string> patterns;
	std::vector<std::string> requested_designs;
	
	readInput(&patterns, &requested_designs);
	
	// print
	std::cout << "PATTERNS:\n";
	for(int i = 0; i < 10; i++)
	{
		std::cout << "[" << patterns[i] << "]\n";
	}
	std::cout << "...\n";
	
	std::cout << "REQUESTED DESIGNS:\n";
	for(int i = 0; i < 10; i++)
	{
		std::cout << "[" << requested_designs[i] << "]\n";
	}
	std::cout << "...\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	int result = numberOfPossibleDesigns(patterns, requested_designs);
	std::cout << ">>> POSSIBLE DESIGNS: [" << result << "]\n";
	
	std::cout << "\n";
	
	long long result2 = numberOfPossibleDesignArrangements(patterns, requested_designs);
	std::cout << ">>> POSSIBLE DESIGN ARRANGEMENTS: [" << result2 << "]\n";	
					
	return true;
}

bool Day19::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = numberOfPossibleDesigns_test();
	if(ok) ok = numberOfPossibleDesignArrangements_test();

	return ok;
}

/**
 * CODE
 */
int Day19::numberOfPossibleDesigns(const std::vector<std::string>& patterns, const std::vector<std::string>& requested_designs)
{
	int count = 0;
	
	for(auto& requested_design : requested_designs)
	{
		bool res = tryPatternsFromBehind(patterns, requested_design);
		
		if(res)
		{
			std::cout << "MATCHING COMBO FOUND\n";		
			count++;
		}
		else
		{
			std::cout << "NO MATCH FOUND\n";
		}
	}
	
	return count;
}

long long Day19::numberOfPossibleDesignArrangements(const std::vector<std::string>& patterns, const std::vector<std::string>& requested_designs)
{
	long long  count = 0;
	
	int max_pattern_size = max_string_length(patterns);
	
	for(auto& requested_design : requested_designs)
	{
		long long res = tryPatterns(patterns, requested_design, 0, max_pattern_size);
		
		if(res > 0)
		{
			std::cout << "MATCHING ARRANGEMENTS FOUND [" << res << "]\n";		
			count += res;
		}
		else
		{
			std::cout << "NO MATCH FOUND\n";
		}
	}
	
	return count;
}

long long Day19::tryPatterns(const std::vector<std::string>& patterns, const std::string& s, const int depth, const int max_pattern_size)
{
	static std::unordered_map<std::string, long long> cache; // the cache is the trick!
	
	if(depth == 0) cache.clear();
	
	if(s.empty()) return 1;
	
	long long count = 0;
	
	for(int i = max_pattern_size; i >= 1; i--)
	{
		if((int)s.size() < i) continue;
		
		std::string search = s.substr(0,i);
		if(std::find(patterns.begin(), patterns.end(), search) != patterns.end())
		{
			const std::string remainder = s.substr(i, s.size() - 1);
			if(cache.count(remainder) == 0)	// if not in cache
			{
				cache[remainder] = tryPatterns(patterns, remainder, depth + 1, max_pattern_size);
			}
			count += cache[remainder];
		}
	}
	
	return count;
}

bool Day19::tryPatternsFromBehind(const std::vector<std::string>& patterns, const std::string& requested_design)
{
	std::priority_queue<std::string, std::vector<std::string>, Comparator> queue;
	
	queue.push(requested_design);
	
	while(!queue.empty())
	{		
		// get and pop topmost entry
		const auto s = queue.top();
		queue.pop();
		
		// are we done?
		if(s.size() == 0)
		{
			return true;
		}
				
		//std::cout << "[" << queue.size() << "] [" << requested_design << "] --> for [" << s << "]\n";
		
		// check patterns
		for(auto& pattern : patterns)
		{
			if(ends_with(s, pattern))
			{
				//std::cout << "  checking [" << pattern << "] --> match\n";
				
				auto ss = s;
				remove_last(ss, (int)pattern.size());
				
				//std::cout << "  --> pushing [" << ss << "] on queue\n";
				
				queue.push(ss);
			}
		}
	}
	
	return false;
}

/**
 * INPUT
 */
bool Day19::readInput(std::vector<std::string> * patterns, std::vector<std::string> * requested_designs)
{
	std::string line;
	
	std::ifstream infile("data/day19.txt");

	patterns->clear();
	requested_designs->clear();
	
	// get patterns
	getline(infile, line);
	std::stringstream ss(line);
	std::string s;
	while (std::getline(ss, s, ','))
	{
		ltrim(s);
		rtrim(s);
		patterns->push_back(s);
	}
	
	// blank line
	getline(infile, line);
	
	// get requested designs	
	while(getline(infile, line))
	{
		requested_designs->push_back(line);
	}	
	
	return true;
}


/**
 * TESTS 
 */
bool Day19::numberOfPossibleDesigns_test()
{
	std::vector<std::string> patterns = {"r", "wr", "b", "g", "bwu", "rb", "gb", "br"};
	std::vector<std::string> requested_designs = {"brwrr",
		                                          "bggr",
		                                          "gbbr",
		                                          "rrbgbr",
		                                          "ubwu",
		                                          "bwurrg",
		                                          "brgr",
		                                          "bbrgwb" };
	
	std::cout << "\n";
	
	int result = numberOfPossibleDesigns(patterns, requested_designs);
	
	if(result == 6)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day19::numberOfPossibleDesignArrangements_test()
{
	std::vector<std::string> patterns = {"r", "wr", "b", "g", "bwu", "rb", "gb", "br"};
	std::vector<std::string> requested_designs = {"brwrr",
		                                          "bggr",
		                                          "gbbr",
		                                          "rrbgbr",
		                                          "ubwu",
		                                          "bwurrg",
		                                          "brgr",
		                                          "bbrgwb" };
	
	std::cout << "\n";
	
	int result = numberOfPossibleDesignArrangements(patterns, requested_designs);
	
	if(result == 16)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
