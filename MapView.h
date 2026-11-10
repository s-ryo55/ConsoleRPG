#pragma once
#include <iostream>
#include "../Model/Map.h"

// =============================================================
//  View : マップの描画（以前の Map::Map_Display）
// =============================================================
class MapView
{
public:
	static void Show(const Map& map)
	{
		char grid[MAP_SIZE][MAP_SIZE];
		map.BuildDisplayGrid(grid);

		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				std::cout << grid[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
};
