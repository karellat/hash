#pragma once
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <bitset>
#include <iostream>
#include "randomGen.h"

using bits = std::bitset<16>;

class hash_function
{
public:
	virtual ~hash_function() = default;
	virtual size_t h(const uint64_t x) = 0;
	virtual void make_random() = 0;
};

class naive_modulo : public hash_function
{
public: 
	naive_modulo(std::size_t m):m(m){}

	virtual size_t h(const uint64_t x) override
	{
		//TODO: make bit mask
		return x % m; 
	}

	virtual void make_random() override
	{
		//TODO: Remove return
		return;
		std::cout << "Make random in kontext of naive_modulo"; 
		throw 1;
	}
	
	std::size_t m; 
};

class multiply_shift : public hash_function
{
public:
	virtual size_t h(const uint64_t x) override
	{
		//TODO: make bit mask
		return (size_t) ((a*x) % U) / (U/m);		
	}
	
	virtual void make_random() override
	{
		uint64_t new_a = rng_next_u64();
		while(new_a % 2 != 1)
			new_a = rng_next_u64();
		a = new_a;
	}

	multiply_shift(const uint64_t U, const std::size_t m, const uint64_t a) : a(a),U(U),m(m)
	{
	}
	uint64_t a; 
	size_t m;
	uint64_t U; 
};

class tabular_fnc : public hash_function
{
public: 

	tabular_fnc(const size_t block_length, const size_t m) : block_length(block_length), block_count(64 / block_length), table_size(m)
	{
		assert(block_length > 0 && block_length < 64);
		//předpokládá m mocninu dvojky 
		output_max = m - 1;
		block_max = 1 << block_length;
		block_max--;
		tab = new size_t*[block_count];
		make_random();
	
	}

	~tabular_fnc()
	{
		for (int i = 0; i < block_count; ++i)
			delete[] tab[i];
	}

	virtual size_t h(const uint64_t x) override
	{
		std::cout << std::endl;
		uint64_t y = x; 
		size_t index = 0;
	
		for (int i = 0; i < block_count; ++i)
		{
			size_t tab_index = y & block_max; 

			assert(tab_index >= 0 && tab_index <= block_max);
			index = index ^ tab[i][tab_index];
			y = y >> block_length;
		}
		return index;
	}

	void make_random() override
	{
		for (int i = 0; i < block_count; ++i)
		{
			tab[i] = new size_t[block_max + 1];
			for (int j = 0; j < block_max + 1; ++j)
			{
				//Mask number to wanted lenght
				tab[i][j] = (size_t)rng_next_u64() & output_max;
			}
		}
	}

	size_t output_max;
	size_t table_size;
	size_t block_max; 
	size_t block_length;
	size_t block_count;
	size_t ** tab;
};