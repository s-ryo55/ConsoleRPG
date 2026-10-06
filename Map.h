#pragma once
#include <iostream>
using namespace std;
#define MAP_SIZE 10

class Map
{
public:
	char map[MAP_SIZE][MAP_SIZE];

	Map() {
		// マップ全体を初期化
		const char initial[MAP_SIZE][MAP_SIZE] = {
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', '#', '#', ' ', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#' },
			{ '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#' },
			{ '#', 'E', '#', '#', ' ', '#', '#', ' ', '#', '#' },
			{ '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' }
		};
		for (int i = 0; i < MAP_SIZE; ++i) {
			for (int j = 0; j < MAP_SIZE; ++j) {
				map[i][j] = initial[i][j];
			}
		}
	}

	void Map_Display() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				cout << map[i][j] << " ";
			}
			cout << std::endl;
		}
	}

};