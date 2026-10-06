#pragma once
#include <iostream>
using namespace std;


class Map
{
	int map[10][10];

public:
	Map() {}

	void Map_Display() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				cout << map[i][j] << " ";
			}
			cout << std::endl;
		}
	}

};