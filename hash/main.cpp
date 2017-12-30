#include <iostream>
#include "hash_table.h"

int main(int argc,char** argv)
{

	linear_table* l = new linear_table(4, new naive_modulo(4)); 
	l->Insert(4);
	l->Insert(8); 
	l->Insert(16);
	l->Insert(32);
	std::cout << "Sardel"; 

	l = new linear_table(4, new multiply_shift(32, 4, 5)); 
	l->Insert(4);
	l->Insert(8);
	l->Insert(16);
	l->Insert(32);
	std::cout << "Sardel";

	l = new linear_table(4, new tabular_fnc(4));
	l->Insert(4);
	l->Insert(8);
	l->Insert(16);
	l->Insert(32);
	std::cout << "Sardel";
}
