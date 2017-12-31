#include <iostream>
#include "hash_table.h"
#include "randomGen.h"

const size_t twoTo20 = 1048576;
const size_t output_cycle = 100;

void printTime(hash_table* table)
{
	std::cout << table->fill_factor()<< ":" << table->average_time() << std::endl;
}

void printSteps(hash_table* table)
{
	std::cout << table->fill_factor() << ":" << table->average_steps() << std::endl;
}

int main(int argc,char** argv)
{
	if(argc < 4)
	{
		std::cout << "Wrong amount of parameters " << std::endl;
		return 1;
	}

	//Set up the random generator with student ID 
	rng_setup(23);

	//Maximum 
	uint64_t universum_max = 0;
	universum_max = ~(universum_max);

	//Block size
	size_t block_size = 8;

	//table size
	size_t m = twoTo20;

	const std::string table = argv[1];
	const std::string func  = argv[2];
	const std::string test  = argv[3];
	
	hash_function* f;
	hash_function* g;

	if(func == "t")
	{
		f = new tabular_fnc(block_size, m);
		g = new tabular_fnc(block_size, m);
	}
	else if (func == "m")
	{
		f = new multiply_shift(universum_max, m, 0);
		g = new multiply_shift(universum_max, m, 0);
		f->make_random();
		g->make_random();
	}
	else if (func == "n")
	{
		f = new naive_modulo(m);
		g = new naive_modulo(m);
	}
	else
	{
		std::cout << "Wrong parameters " << std::endl;
		return 1;
	}

	hash_table* t; 
	if(table == "c")
	{
		if(func == "n")
		{
			std::cout << "Naive functions cannot be part of cucoo" << std::endl;
			return 1;
		}

		t = new cuckoo_table(m, f, g);
	}
	else if (table == "l")
	{
		t = new linear_table(m, f);
	}
	else
	{
		std::cout << "Wrong parameters " << std::endl;
		return 1;
	}

	if(test == "time")
	{
		for (int i = 0; i < m; ++i)
		{
			t->Insert(rng_next_u64());
			if (i % output_cycle == 0)
				printTime(t);
		}
	}
	else if (test == "steps")
	{
		for (int i = 0; i < m; ++i)
		{
			t->Insert(rng_next_u64());
			if (i % output_cycle == 0)
				printTime(t);
		}
	}
	else if (test == "seq")
	{
		std::cout << "Not implemented" << std::endl;
		return 1;
	}


}

