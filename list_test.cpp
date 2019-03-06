#include<iostream>
#include"list.h"
#include"vector.h"


int main(){
	MiniSTL::list<int> l1 = {1,3,5,7,9};
	MiniSTL::list<int> l2;
	l2 = l1;
	
	printf("print {1,3,5,7,9}: \n");
	for(auto &a : l2)
		std::cout << a << " ";
	std::cout << std::endl;

	printf("print insert(l2.begin(), 0):\n ");
	l2.insert(l2.begin(), 0);
	for(auto &a : l2)
		std::cout << a << " ";
	std::cout << std::endl;

	printf("print insert(l2.end(), 11):\n");
	l2.insert(l2.end(), 11);
	for(auto &a : l2)
		std::cout << a << " ";
	std::cout << std::endl;
	
	printf("print insert(++l2.begin(), {2,4,6}):\n");
	MiniSTL::vector<int> v {2,4,6};
	l2.insert(++l2.begin(), v.begin(), v.end());
	for(auto &a : l2)
		std::cout << a << " ";
	std::cout << std::endl;

	printf("print erase(l2.begin()): \n");
	l2.erase(l2.begin());
	for(auto &a : l2)
		std::cout << a << " ";
	std::cout << std::endl;
	
	printf("print erase(l2.end()-1): \n");
	l2.erase(--l2.end());
	for(auto &a : l2)
		std::cout << a << " ";
	std::cout << std::endl;
}
