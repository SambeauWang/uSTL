//#include "./include/stl_config.h"
#include <iostream>
#include <string>
//#include "ustl_construct.h"
//#include "ustl_alloc.h"
//#include "ustl_iterator_base.h"
//#include "ustl_vector.h"
#include "ustl_vector.h"

using namespace std;

int main(){
	int a;
	/*int nums[] = { 0, 1, 2, 3, 4, 5 };
	uvector<int> v(nums, nums + 6);*/
	string nums[] = { "a", "b", "c", "d", "e", "f" };
	uvector<string> v(nums, nums + 6);

	for (uvector<string>::iterator i = v.begin(); i != v.end(); ++i)
		cout << *i << endl;
	cin >> a;
	/*int i;
	int* nums = (int *)ualloc::allocate(10 * sizeof(int));

	for (i = 0; i < 10; i++){
		uconstruct(nums + i, i);
	}

	for (i = 0; i < 10; i++)
		cout << *(nums + i) << endl;

	cin >> i;
	return 0;*/
}