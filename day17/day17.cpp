#include "day17.hpp"
		
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <stdlib.h>
#include <climits>

Computer::Computer()
{
	this->regA = 0;
	this->regB = 0;
	this->regC = 0;
	
	this->ip = 0;
}

std::vector<unsigned int> Computer::execute(const std::vector<unsigned int>& program)
{
	// start program from 0
	this->ip = 0;
	this->output.clear();
	
	while(this->ip < (int)program.size())
	{
		tick(program);
	}
	
	return this->output;
}

void Computer::tick(const std::vector<unsigned int>& program)
{
	Instruction  i = (Instruction)program[this->ip];
	unsigned int o = program[this->ip + 1];
	
	unsigned long numerator = 0;
	unsigned long denominator = 0;
	
	switch(i)
	{
		case ADV:
		
			// The adv instruction (opcode 0) performs division. The numerator is the value in
			// the A register. The denominator is found by raising 2 to the power of the instruction's
			// combo operand. (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would 
			// divide A by 2^B.) The result of the division operation is truncated to an integer and 
			// then written to the A register.
		
			o = combo(o);
		
			//std::cout << "ADV: [" << this->regA << "/" << o << ":" << (1<<o) << "] = [" << (this->regA/(1<<o)) << "]\n";
		
			numerator   = this->regA;
			denominator = (1 << o);	// 2 to the power of o ... o=0->1, o=1->2, o=2->4, o=3->8
			this->regA = (numerator / denominator);
			
			this->ip += 2;
			
			break;
			
		case BXL:
		
			// The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's 
			// literal operand, then stores the result in register B.
		
			//std::cout << "BXL: [" << this->regB << "^" << o << "] = [" << (this->regB ^ o) << "]\n";
		
			this->regB = (this->regB ^ o);
			
			this->ip += 2;
			
			break;
			
		case BST:
		
			// The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby 
			// keeping only its lowest 3 bits), then writes that value to the B register.
			
			o = combo(o);
			
			//std::cout << "BST: [" << o << "%8] = [" << (o%8) << "]\n";
			
			this->regB = (o % 8);
			
			this->ip += 2;
			
			break;
			
		case JNZ:
		
			// The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register 
			// is not zero, it jumps by setting the instruction pointer to the value of its literal operand; 
			// if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
			
			//std::cout << "JNZ: [" << this->regA << "->" << o << "]\n";
			
			if(this->regA != 0)
			{
				this->ip = o;
			}
			else
			{
				this->ip += 2;
			}
			
			break;
			
		case BXC:
		
			// The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then 
			// stores the result in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
			
			//std::cout << "BXC: [" << this->regB << "^" << this->regC << "] == [" << (this->regB ^ this->regC) << "]\n";
			
			this->regB = (this->regB ^ this->regC);
			
			this->ip += 2;
			
			break;
			
		case OUT:
		
			// The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. 
			// (If a program outputs multiple values, they are separated by commas.)
			
			o = combo(o);
			
			//std::cout << "OUT: [" << o << "%8] == [" << (o%8) << "]\n";
			
			this->output.push_back(o % 8);
			
			this->ip += 2;
			
			break;
			
		case BDV:
		
			// The bdv instruction (opcode 6) works exactly like the adv instruction except that the result 
			// is stored in the B register. (The numerator is still read from the A register.)
			
			o = combo(o);
		
			//std::cout << "BDV: [" << this->regA << "/" << o << ":" << (1<<o) << "] = [" << (this->regA/(1<<o)) << "]\n";
		
			numerator   = this->regA;
			denominator = (1 << o);	// 2 to the power of o ... o=0->1, o=1->2, o=2->4, o=3->8
			this->regB = (numerator / denominator);
			
			this->ip += 2;
			
			break;
			
		case CDV:
		
		    // The cdv instruction (opcode 7) works exactly like the adv instruction except that the result
		    // is stored in the C register. (The numerator is still read from the A register.)
			
			o = combo(o);
		
			//std::cout << "CDV: [" << this->regA << "/" << o << ":" << (1<<o) << "] = [" << (this->regA/(1<<o)) << "]\n";
			
			numerator   = this->regA;
			denominator = (1 << o);	// 2 to the power of o ... o=0->1, o=1->2, o=2->4, o=3->8
			this->regC = (numerator / denominator);
			
			this->ip += 2;
			
			break;
	}
}

unsigned long Computer::combo(unsigned long o)
{
	if(o <= 3) return o;
	else if(o == 4) return this->regA;
	else if(o == 5) return this->regB;
	else if(o == 6) return this->regC;
	else if(o == 7) { std::cout << "INVALID COMBO OPERAND\n"; }
	
	return 0;
}

void Computer::reset()
{
	this->regA = 0;
	this->regB = 0;
	this->regC = 0;
	
	this->ip = 0;
	
	this->output.clear();
}

bool Day17::run()
{
    // GET DATA
	std::cout << "\n";
	std::cout << "INPUT DATA:\n";
	
	std::vector<unsigned int> input;
	unsigned long A,B,C;
	
	readInput(&input,&A,&B,&C);
	
	// print
	std::cout << "INPUT:\n";
	std::cout << "reg A: [" << A << "]\n";
	std::cout << "reg B: [" << B << "]\n";
	std::cout << "reg C: [" << C << "]\n";
	std::cout << "program: [";
	for(int i = 0; i < 10; i++)
	{
		std::cout << input[i] << ",";
	}
	std::cout << " ...]\n";
	
	// GO
	std::cout << "\n";
	std::cout << "OUTPUT DATA:\n";
	
	std::vector<unsigned int> result = getChronoSpatialComputerOutput(input,A,B,C);
	std::string s = outputToString(result);
	std::cout << ">>> OUTPUT: [" << s << "]\n";
	
	std::cout << "\n";
	
	unsigned long result2 = getChronoSpatialComputerProgramCopyValue(input,A,B,C);
	std::cout << ">>> LOWEST A VALUE: [" << result2 << "]\n";
					
	// 140736435103904 is too low...
	// 140736662680208 is also too low...
	// 140736998937616
					
	return true;
}

bool Day17::test()
{
	bool ok = true;
	
    std::cout << "TESTS:\n";    
    
	if(ok) ok = getChronoSpatialComputerOutput_test();
	if(ok) ok = getChronoSpatialComputerProgramCopyValue_test();

	
	return ok;
}

/**
 * CODE
 */
std::vector<unsigned int> Day17::getChronoSpatialComputerOutput(const std::vector<unsigned int>& program, const unsigned long A, const unsigned long B, const unsigned long C)
{
	Computer c;
	c.regA = A;
	c.regB = B;
	c.regC = C;
	
	return c.execute(program);
}


unsigned long Day17::getChronoSpatialComputerProgramCopyValue(const std::vector<unsigned int>& program, const unsigned long A, const unsigned long B, const unsigned long C)
{
	// So brute forcing didn't work - again. Let's go deeper then...
	//
	// We know:
	// 1) Only the last 3 bit of register A matter in the output (see OUT instruction)
	// 2) The number in register A gets smaller until program halts (last instruction is "3,0" --> JNZ of register A)
	//
	// so lets make a solution that "solves" the program, from a register A input
	// that makes use of the above points
	
	unsigned long best_A = ULONG_MAX;
	bool done = false;
	
	solveByPart(program, 0, (int)program.size() - 1, &best_A, &done); // backwards
	
	return best_A;
}

void Day17::solveByPart(const std::vector<unsigned int>& program, unsigned long current_A, int index, unsigned long * best_A, bool * done)
{
	
	int depth = (int)program.size() - index;
	std::string header;
	for(int i = 0; i < depth; i++)
	{
		header.append(">");
	}
	
	if(index < 0)
	{
		*best_A = std::min(current_A, *best_A);
		return;
	}
	
	for(unsigned long rem = 0; rem < 8; rem++)    // only increment last 3 bits
	{
		unsigned long next_A = (current_A << 3) + rem;   // shift the other number up by 3 bits
		
		Computer c;
		c.regA = next_A;
		
		std::vector<unsigned int> output = c.execute(program);
		
		std::cout << header << "trying next_A [" << next_A << "] --> [" << outputToString(output) << "] --> ";
		
		if(compareLastElements(program, output))
		{
			std::cout << "match\n";
			
			if(output.size() == program.size())
			{
				std::cout << "<<< FULL MATCH @ " << next_A << " >>>\n";
				std::cout << "program: [" << outputToString(program) << "]\n";
				std::cout << "output:  [" << outputToString(output) << "]\n";
				*done = true;
				*best_A = next_A;
				return;
			}
			
			solveByPart(program, next_A, index - 1, best_A, done);
			
			if(*done) return;
		}
		else
		{
			std::cout << "no match\n";
		}
	}
}

bool Day17::compareLastElements(const std::vector<unsigned int>& a, const std::vector<unsigned int>& b)
{	
	int a_last = (int)a.size() - 1;
	int b_last = (int)b.size() - 1;
	
	// checking for "b" at end of "a"
	for(int i = 0; i < (int)b.size(); i++)
	{
		//std::cout << "                comparing [" << b[b_last - i] << "=" << a[a_last - i] << "] --> [" << (b[b_last - i] == a[a_last - i]?"true":"false") << "]\n";
		if(b[b_last - i] != a[a_last - i]) return false;
	}

	return true;
}

std::string Day17::outputToString(const std::vector<unsigned int>& v)
{
	std::string out;
	
	for(int i = 0; i < (int)v.size() - 1; i++)
	{
		out.append(std::to_string(v[i]));
		out.append(",");
	}
	out.append(std::to_string(v.back()));
	
	return out;	
}

/**
 * INPUT
 */
bool Day17::readInput(std::vector<unsigned int> * input, unsigned long * A, unsigned long * B, unsigned long * C)
{
	std::string line;
	
	std::ifstream infile("data/day17.txt");

	input->clear();
	
	// reg A
	getline(infile, line);
	if(sscanf(line.c_str(), "Register A: %lu", A) != 1) return false;
	
	// reg B
	getline(infile, line);
	if(sscanf(line.c_str(), "Register B: %lu", B) != 1) return false;
	
	// reg C
	getline(infile, line);
	if(sscanf(line.c_str(), "Register C: %lu", C) != 1) return false;
	
	// empty
	getline(infile, line);
	
	// program
	getline(infile, line);
	line.erase(0,9);
	
	//std::cout << "program: [" << line << "]\n";
	
	std::stringstream ss(line);
	std::string s;

	while (std::getline(ss, s, ','))
	{
		unsigned int val = std::stoi(s);
		input->push_back(val);
	}
	
	return true;
}


/**
 * TESTS 
 */
bool Day17::getChronoSpatialComputerOutput_test()
{
	bool ok = true;
	
	std::cout << "\n";
	
	//
	// test 1
	//
	if(ok)
	{
		Computer c;
		c.regC = 9;
		std::vector<unsigned int> program = {2,6};
		
		c.execute(program);
		
		if(c.regB == 1)
		{
			//std::cout << "test 1 ok\n";
		}
		else
		{
			std::cout << "test 1 failed\n";
			ok = false;
		}
	}
	
	// 
	// test 2
	//
	if(ok)
	{
		Computer c;
		c.regA = 10;
		std::vector<unsigned int> program = {5,0,5,1,5,4};
		std::vector<unsigned int> out;
		std::vector<unsigned int> out_exp = {0,1,2};
		
		out = c.execute(program);
		
		if(out == out_exp)
		{
			//td::cout << "test 2 ok\n";
		}
		else
		{
			std::cout << "test 2 failed\n";
			ok = false;
		}
	}
	
	// 
	// test 3
	//
	if(ok)
	{
		Computer c;
		c.regA = 2024;
		std::vector<unsigned int> program = {0,1,5,4,3,0};
		std::vector<unsigned int> out;
		std::vector<unsigned int> out_exp = {4,2,5,6,7,7,7,7,3,1,0};
		
		out = c.execute(program);
		
		if(out == out_exp && c.regA == 0)
		{
			//std::cout << "test 3 ok\n";
		}
		else
		{
			std::cout << "test 3 failed\n";
			ok = false;
		}
	}
	
	//
	// test 4
	//
	if(ok)
	{
		Computer c;
		c.regB = 29;
		std::vector<unsigned int> program = {1,7};
		
		c.execute(program);
		
		if(c.regB == 26)
		{
			//std::cout << "test 4 ok\n";
		}
		else
		{
			std::cout << "test 4 failed\n";
			ok = false;
		}
	}
	
	//
	// test 5
	//
	if(ok)
	{
		Computer c;
		c.regB = 2024;
		c.regC = 43690;
		std::vector<unsigned int> program = {4,0};
		
		c.execute(program);
		
		if(c.regB == 44354)
		{
			//std::cout << "test 5 ok\n";
		}
		else
		{
			std::cout << "test 5 failed\n";
			ok = false;
		}
	}
	
	//
	// test 6
	//
	if(ok)
	{
		Computer c;
		c.regA = 729;
		std::vector<unsigned int> program = {0,1,5,4,3,0};
		std::vector<unsigned int> out;
		std::vector<unsigned int> out_exp = {4,6,3,5,6,3,5,2,1,0};
		
		out = c.execute(program);
		
		if(out == out_exp)
		{
			//std::cout << "test 6 ok\n";
		}
		else
		{
			std::cout << "test 6 failed\n";
						
			ok = false;
		}
	}
	
	if(ok)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}

bool Day17::getChronoSpatialComputerProgramCopyValue_test()
{

	std::cout << "\n";
	
	long long A = 2024;
	long long B = 0;
	long long C = 0;
	std::vector<unsigned int> program = {0,3,5,4,3,0};
		
	long long res = getChronoSpatialComputerProgramCopyValue(program, A, B, C);
		
	if(res == 117440)
	{
		std::cout << "Test [" << __func__ << "] PASSED\n";
		return true;
	}
	
	std::cout << "Test [" << __func__ << "] FAILED\n";

	return false;
}
