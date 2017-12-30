#pragma once
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <bitset>
#include <iostream>

using bits = std::bitset<16>;

class hash_function
{
public:
	virtual ~hash_function() = default;
	virtual size_t h(const uint64_t x) = 0; 
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
	std::size_t m; 
};

class multiply_shift : public hash_function
{
public:
	virtual  size_t h(const uint64_t x) override
	{
		//TODO: make bit mask
		return (size_t) ((a*x) % U) / (U/m);		
	}
	//TODO: Celo číselné dělení? a nemůže se stát overflow??
	/*size_t multiplicative_hash(uint64_t a, uint64_t x) {
		return
	}*/
	multiply_shift(const uint64_t U, const std::size_t m, const uint64_t a) : a(a),U(U),m(m) {}
	uint64_t a; 
	size_t m;
	uint64_t U; 
};

class tabular_fnc : public hash_function
{
public: 
	tabular_fnc(const size_t block_length) : block_length(block_length),block_count(64/block_length)
	{
		assert(block_length > 0 && block_length < 64);
		block_max = 1 << block_length;
		block_max--;

		tab = new size_t[block_count]; 
		
	}
	virtual size_t h(const uint64_t x) override
	{
		uint64_t y = x; 
		size_t index = 0;

		for (int i = 0; i < block_count; ++i)
		{
			std::cout << bits(y) << std::endl;
			std::cout << bits(block_max) << std::endl << std::endl;

			size_t tab_index = y && block_max; 
			
			std::cout << bits(tab_index) << std::endl << std::endl;
			assert(tab_index >= 0 && tab_index <= block_max);
			std::cout << "[" << tab_index << "] = " << tab[tab_index] << std::endl << std::endl;
			std::cout << bits(index) << std::endl; 
			std::cout << bits(tab[tab_index]) << std::endl;
			std::cout << std::endl;
			index = index ^ tab[tab_index];
			std::cout << bits(index) << std::endl << std::endl;
			y = y >> block_length;
		}
		return index;


	}


	size_t block_max; 
	size_t block_length;
	size_t block_count;
	size_t* tab;
};

