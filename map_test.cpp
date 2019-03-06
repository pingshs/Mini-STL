#include"map.h"
#include<vector>
#include<cstdio>

int main(){
	MiniSTL::map<int, int> m({0,0});
	std::vector<MiniSTL::pair<int, int>> v{{10,1}, {85,2}, {15, 3}, {70, 4}, {20,5}, {60, 6}, 

			{30,7}, {50,8}, {65,9}, {80,10}, {90,11}, {40,12},
			{5,13}, {55, 14}};

	for(auto& a: v)
		m.insert(a);

	m.PrintInorder_Iteratorly();
	printf("\n");
	m.PrintLevel();
	printf("\n");
	m.erase({30, 7});
	m.PrintInorder_Iteratorly();
	printf("\n");
	m.PrintLevel();
	printf("\n");

	for(auto& a : m){
		std::cout << a.first;
		printf("\t");
	}
	std::cout << std::endl;

}
