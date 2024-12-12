#pragma once

#include <vector>

class Node
{
	public:
		std::vector<Node *> children;
		
		int x;
		int y;
		int value;
		
		Node(int _x, int _y, int _val);

};

class Day10
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<std::vector<int>> * map);
		
		int trailheadScoreSum(std::vector<std::vector<int>> map);
		int trailheadRatingSum(std::vector<std::vector<int>> map);
		
		
		Node * walkRecursive(std::vector<std::vector<int>>& map, int x, int y, int val_prev);
		
		void placeOnMap(std::vector<std::vector<char>>& map, Node * p, int depth, int until);
		void printMap(std::vector<std::vector<char>>& map);
		
		void countTrailheadScore(Node * p, std::vector<std::vector<int>>& coordinates);
		void countTrailheadRating(Node * p, int& count);
		
		bool trailheadScoreSum_test();
		bool trailheadRatingSum_test();
};
