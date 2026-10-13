#pragma once
#include <iostream>
#include <vector>
using namespace std;
#define MAP_SIZE 20

// 敵の情報を管理する構造体
struct EnemyData
{
	int row;
	int col;
	bool isAlive;  // 敵が生存しているかどうか
	int hp;        // 敵のHP
	int maxHp;     // 敵の最大HP
};

class Map
{
public:
	char map[MAP_SIZE][MAP_SIZE];
	vector<EnemyData> enemies;  // 敵の情報を個別に管理

	Map() {
		// マップ全体を初期化
		const char initial[MAP_SIZE][MAP_SIZE] = {
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', '#', '#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', '#', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', 'B', '#' },
			{ '#', 'E', '#', '#', ' ', '#', '#', ' ', '#', '#' ,' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', 'E', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', ' ', 'S', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', 'H', '#', ' ', ' ', ' ', 'R', '#' },
			{ '#', ' ', ' ', ' ', '#', ' ', '#', '#', '#', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', '#'},
			{ '#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', '#', 'E', '#', 'E', '#', '#', '#', '#' },
			{ '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', '#', ' ', ' ', '#', 'E', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', ' ', ' ', '#', 'E', '#', ' ', ' ', ' ', ' ', '#' },
			{ '#', 'E', '#', '#', 'E', '#', '#', ' ', '#', '#' , ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ','#' },
			{ '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' , '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' }
		};
		
		for (int i = 0; i < MAP_SIZE; ++i) {
			for (int j = 0; j < MAP_SIZE; ++j) {
				map[i][j] = initial[i][j];
				
				// 敵の位置を記録する
				if (initial[i][j] == 'E') {
					EnemyData enemy;
					enemy.row = i;
					enemy.col = j;
					enemy.isAlive = true;
					enemy.maxHp = 30;  // 敵の最大HP
					enemy.hp = enemy.maxHp;  // 現在のHP
					enemies.push_back(enemy);
				}
			}
		}
	}

	// 敵を検索する（座標から）
	EnemyData* FindEnemyAt(int row, int col) {
		for (auto& enemy : enemies) {
			if (enemy.row == row && enemy.col == col && enemy.isAlive) {
				return &enemy;
			}
		}
		return nullptr;
	}

	// 敵にダメージを与える
	bool DamageEnemyAt(int row, int col, int damage) {
		EnemyData* enemy = FindEnemyAt(row, col);
		if (enemy != nullptr) {
			enemy->hp -= damage;
			if (enemy->hp <= 0) {
				enemy->hp = 0;
				enemy->isAlive = false;
				map[row][col] = ' ';  // マップから敵を削除
				return true;  // 敵を倒した
			}
			return false;  // 敵はまだ生きている
		}
		return false;
	}

	// 指定した敵を倒す（座標で指定）- 戦闘なしで直接倒す場合用
	void DefeatEnemyAt(int row, int col) {
		EnemyData* enemy = FindEnemyAt(row, col);
		if (enemy != nullptr) {
			enemy->isAlive = false;
			enemy->hp = 0;
			map[row][col] = ' ';  // マップから敵を削除
		}
	}

	// 敵のHPをリセットする（新しい敵と遭遇する場合用）
	void ResetEnemyHP(int row, int col) {
		EnemyData* enemy = FindEnemyAt(row, col);
		if (enemy != nullptr) {
			enemy->hp = enemy->maxHp;
		}
	}

	// 生存している敵の数を返す
	int GetAliveEnemyCount() const {
		int count = 0;
		for (const auto& enemy : enemies) {
			if (enemy.isAlive) {
				count++;
			}
		}
		return count;
	}

	// マップを表示（生存している敵のみ表示）
	void Map_Display() {
		// 表示用に一時的なマップを作成
		char displayMap[MAP_SIZE][MAP_SIZE];
		for (int i = 0; i < MAP_SIZE; ++i) {
			for (int j = 0; j < MAP_SIZE; ++j) {
				displayMap[i][j] = map[i][j];
			}
		}

		// 生存している敵を表示マップに追加
		for (const auto& enemy : enemies) {
			if (enemy.isAlive) {
				displayMap[enemy.row][enemy.col] = 'E';
			}
		}

		// 表示
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				cout << displayMap[i][j] << " ";
			}
			cout << std::endl;
		}
	}
};