#include<iostream>
#include"unordered_set.h"

int main(){
	MiniSTL::unordered_set<int> s = {10, 2, 3, 4,5,6};

	for(auto &a : s)
		std::cout << a << " ";
	std::cout << std::endl;

	s.insert(10);
	for(auto &a : s)
		std::cout << a << " ";
	std::cout << std::endl;

	s.insert(11);
	for(auto &a : s)
		std::cout << a << " ";
	std::cout << std::endl;

	s.insert(15);
	for(auto &a : s)
		std::cout << a << " ";
	std::cout << std::endl;
}
