#pragma once
#include <cstdint>
#include <cstdio>
#include "hash_function.h"
#include <cstring>
#include <iostream>
#include <cassert>

class hash_table
{
public:
	virtual ~hash_table() = default;
	virtual void Insert(uint64_t x) = 0; 
};


class linear_table : public hash_table
{
public: 

	linear_table(const std::size_t size, hash_function* h): h(h),count(0), size(size)
	{
		table = new uint64_t[size]; 
		memset(table, 0, size * sizeof(uint64_t));
	}

	~linear_table()
	{
		delete[]table;
		delete h; 
	}

	virtual void Insert(uint64_t x) override
	{
		assert(x != 0);

		size_t i = h->h(x);

		while( i < size)
		{
			if(table[i] == 0)
			{
				count++; 
				table[i] = x; 
				return;
			}
			i = (i+1) % size;
		}
#ifndef NDEBUG
		std::cout << "Insert failed, hashtable filled "; 
#endif
		//TODO: rehash 
	}
	
	uint64_t* table; 
	hash_function* h;
	size_t count; 
	size_t size; 
	
};

class cuckoo_table : public hash_table
{
public: 
	cuckoo_table(const size_t size, hash_function* f, hash_function* a) : f(f),a(a),count(0),size(size)
	{
		table = new uint64_t[size];
	}

	uint64_t* table; 
	hash_function* f; 
	hash_function* a; 
	size_t count;
	size_t size; 


};

