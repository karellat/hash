#pragma once
#include <cstdint>
#include <cstdio>
#include "hash_function.h"
#include <cstring>
#include <iostream>
#include <cassert>
#include <smmintrin.h>
#include <chrono>

class hash_table
{
public:
	virtual ~hash_table() = default;
	virtual void Insert(uint64_t x) = 0; 
	virtual double average_time() = 0;
	virtual double average_steps() = 0; 
	virtual	double fill_factor() = 0; 
	virtual size_t count() = 0;
};


class linear_table : public hash_table
{
public: 

	linear_table(const std::size_t size, hash_function* h): h(h),_count(0), size(size)
	{
		table = new uint64_t[size]; 
		memset(table, 0, size * sizeof(uint64_t));
	}

	~linear_table()
	{
		delete[]table;
		delete h; 
	}

	void Insert(uint64_t x) override
	{
		insert_count++;
		auto start = std::chrono::high_resolution_clock::now();

		assert(x != 0);

		std::size_t i = h->h(x);

		while( i < size)
		{
			if(table[i] == 0)
			{
				_count++; 
				table[i] = x; 

				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> diff = end - start;
				total_insert_time += diff.count();
				return;
			}

			steps_count++;
			i = (i+1) % size;
		}
#ifndef NDEBUG
		std::cout << "Insert failed, hashtable filled "; 
#endif
		throw 1;
	}
	
	double average_time() override 
	{
		if (insert_count == 0)
			return 0;

		return total_insert_time / (double)insert_count;
	}

	double average_steps() override
	{
		if (insert_count == 0)
			return 0;

		return steps_count / (double)insert_count;
	}

	double fill_factor() override
	{
		return _count / (double)size;
	}

	size_t count() override
	{
		return _count;
	}


	uint64_t* table; 
	hash_function* h;
	std::size_t _count; 
	std::size_t size;
	std::size_t insert_count = 0; 
	std::size_t steps_count = 0;
	double total_insert_time = 0.0;
};

class cuckoo_table : public hash_table
{
public: 
	cuckoo_table(const std::size_t size, hash_function* f, hash_function* a) : f(f),g(a),_count(0),size(size)
	{
		table = new uint64_t[size];
		memset(table, 0, size * sizeof(uint64_t));
		max_attempts = ceil(log2(size) * 6);
	}

	~cuckoo_table()
	{
		delete[] table;
		delete f;
		delete g;
	}

	void Insert(uint64_t x) override
	{		
		
		auto start = std::chrono::high_resolution_clock::now();

		insert_count++;
		uint64_t item = x; 
		_count++;
		std::size_t index = f->h(x);

		if(_count > size)
		{
			std::cout << "Hashtable filled, error" << std::endl;
			throw 1;
		}

		if(table[index] == 0)
		{
			table[index] = x; 
			return; 
		}
		else
		{
			steps_count++;
			item = table[index];
			table[index] = x;
		}

		for (int i = 0; i < max_attempts; ++i)
		{
			std::size_t a_index = f->h(item);	
			std::size_t b_index = g->h(item);

			assert(a_index == index || b_index == index); 

			if (a_index == index)
				index = b_index;

			if(table[index] == 0)
			{	
				table[index] = item;
				return;
			}
			else
			{
				steps_count++; 
				uint64_t j = table[index];
				table[index] = item; 
				item = j; 
			}
		}

		//Insert item into random place
		for (int i = 0; i < size; ++i)
		{
			if(table[i] == 0)
			{
				table[i] = item;
				break;
			}
		}

		rehash();

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		total_insert_time += diff.count();
	}

	void rehash()
	{

#ifndef NDEBUG
		std::cout << "Cuckoo rehashing " << std::endl;
#endif

		if(rehashing)
		{
			roll_back = true; 
			std::cout << "Roll Back Cuckoo rehashing " << std::endl;
			return;
		}

		rehashing = true;
		uint64_t* old_table = table; 
		
		table = new uint64_t[size];
		
		roll_back = true;

		while (roll_back) 
		{
			roll_back = false;
			memset(table, 0, size * sizeof(uint64_t));
			_count = 0;
			f->make_random();
			g->make_random();

			for (int i = 0; i < size; ++i)
			{
				if (old_table[i] != 0)
					this->Insert(old_table[i]);
				if (roll_back)
					break;
			}
		}
		delete[] old_table;
		rehashing = false; 
	}

	double average_time() override
	{
		if (insert_count == 0)
			return 0;

		return total_insert_time / (double)insert_count;
	}

	double average_steps() override
	{
		if (insert_count == 0)
			return 0;

		return steps_count / (double)insert_count;
	}

	double fill_factor() override
	{
		return _count / (double)size;
	}

	size_t count() override
	{
		return _count;
	}

	bool roll_back = false; 
	bool rehashing = false;
	uint64_t* table; 
	hash_function* f; 
	hash_function* g; 
	std::size_t _count;
	std::size_t size; 
	std::size_t max_attempts;
	std::size_t steps_count = 0;
	std::size_t insert_count = 0;
	double total_insert_time = 0.0;
};