#pragma once

#include <vector>
#include <string>

class Computer
{
	public:
		
		enum Instruction
		{
			ADV = 0,
			BXL = 1,
			BST = 2,
			JNZ = 3,
			BXC = 4,
			OUT = 5,
			BDV = 6,
			CDV = 7
		};
		
		unsigned long regA;
		unsigned long regB;
		unsigned long regC;
		
		int ip;
		
		std::vector<unsigned int> output;
		
		Computer();
	
		std::vector<unsigned int> execute(const std::vector<unsigned int>& program);
		void tick(const std::vector<unsigned int>& program);
		void reset();
		
		unsigned long combo(unsigned long o);
};

class Day17
{
	public:

		bool run();
		bool test();

	private:
	
		bool readInput(std::vector<unsigned int> * input, unsigned long * A, unsigned long * B, unsigned long * C);

		std::vector<unsigned int> getChronoSpatialComputerOutput(const std::vector<unsigned int>& program, const unsigned long A, const unsigned long B, const unsigned long C);
		unsigned long getChronoSpatialComputerProgramCopyValue(const std::vector<unsigned int>& program, const unsigned long A, const unsigned long B, const unsigned long C);

		std::string outputToString(const std::vector<unsigned int>& v);
		
		void solveByPart(const std::vector<unsigned int>& program, unsigned long current_A, int index, unsigned long * best_A, bool * done);
		bool compareLastElements(const std::vector<unsigned int>& a, const std::vector<unsigned int>& b);

		bool getChronoSpatialComputerOutput_test();
		bool getChronoSpatialComputerProgramCopyValue_test();

};
