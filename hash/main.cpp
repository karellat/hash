#include <iostream>
#include "hash_table.h"

int main(int argc,char** argv)
{
	hash_table * l; 
	l = new linear_table(4, new naive_modulo(4)); 
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
	l->Insert(324);
	l->Insert(8);
	l->Insert(16);
	l->Insert(32);
	std::cout << "Sardel";

	cuckoo_table * t = new cuckoo_table(8, new naive_modulo(8), new naive_modulo(8));
	t->Insert(1);
	t->Insert(2);
	t->Insert(3);
	t->Insert(11);
	t->Insert(4);
	std::cout << "Sardel";
	
}
