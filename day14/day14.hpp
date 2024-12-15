#pragma once

#include <vector>

class Bathroom
{
	public:
		int width;
		int height;
		
		Bathroom(int _width, int _height);
};

class Robot
{
	public:
		int x;
		int y;
		int vx;
		int vy;
		
		Robot(int _x, int _y, int _vx, int _vy);
		
		void move(Bathroom& bathroom);
};

class Day14
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<Robot *> * robots);

		long calculateSafetyFactor(std::vector<Robot *> robots, Bathroom bathroom, int iterations);
		int findEasterEgg(std::vector<Robot *> robots, Bathroom bathroom, int max_iterations);

		bool detectCluster(std::vector<Robot *>& robots, Bathroom& bathroom);
		std::vector<int> calculateRobotsInQuadrants(std::vector<Robot *>& robots, Bathroom& bathroom);
		
		void printBathroom(std::vector<Robot *>& robots, Bathroom& bathroom);


		bool calculateSafetyFactor_test();

		
};
