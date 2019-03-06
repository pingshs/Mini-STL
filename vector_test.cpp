#include<list>
#include<iostream>
#include<cstdio>
#include"vector.h"

int main(){
	MiniSTL::vector<int> v1 = {1,3,5,7,9};
	MiniSTL::vector<int> v2;
	v2 = v1;

	printf("输出v2：\n");
	for(auto & a: v2)
		std::cout << a << " ";
	std::cout << std::endl;

v2.resize(100);
	printf("insert(v2.begin(), 0): \n");
	v2.insert(v2.end(), 0);
	for(auto & a: v2)
		std::cout << a << " ";
	std::cout << std::endl;

	printf("insert(v2.end(), 9): \n");
	v2.insert(v2.end(), 9);
	for(auto & a: v2)
		std::cout << a << " ";
	std::cout << std::endl;

	printf("insert(v2.end(), {10,11,12}): \n");
	MiniSTL::vector<int> l = {10, 11, 12};
	v2.insert(v2.end(), l.begin(), l.end());
	for(auto & a: v2)
		std::cout << a << " ";
	std::cout << std::endl;

	v2.push_back(30);
	v2.erase(v2.end());

	printf("erase(v2.begin(), v2.end()): \n");
	v2.erase(v2.begin(), v2.end());
	for(auto & a: v2)
		std::cout << a << " ";
	std::cout << std::endl;


	return 0;
}
