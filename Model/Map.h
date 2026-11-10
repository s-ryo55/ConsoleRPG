#pragma once
#include <vector>

// =============================================================
//  Model : マップと敵の配置データ（表示は MapView が担当）
// =============================================================

constexpr int MAP_SIZE = 20;

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
	Map()
	{
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
				tiles[i][j] = initial[i][j];

				// 敵の位置を記録する
				if (initial[i][j] == 'E') {
					EnemyData enemy;
					enemy.row = i;
					enemy.col = j;
					enemy.isAlive = true;
					enemy.maxHp = 30;
					enemy.hp = enemy.maxHp;
					enemies.push_back(enemy);
				}
			}
		}
	}

	// ---- マス目の読み書き ----
	char GetTile(int row, int col) const { return tiles[row][col]; }
	void SetTile(int row, int col, char c) { tiles[row][col] = c; }

	// 指定した文字のマスをすべて別の文字に置き換える（例：ボス 'B' を消す）
	void ReplaceTiles(char from, char to)
	{
		for (int i = 0; i < MAP_SIZE; ++i) {
			for (int j = 0; j < MAP_SIZE; ++j) {
				if (tiles[i][j] == from) {
					tiles[i][j] = to;
				}
			}
		}
	}

	// ---- 敵の管理 ----
	// 敵を検索する（座標から）
	EnemyData* FindEnemyAt(int row, int col)
	{
		for (auto& enemy : enemies) {
			if (enemy.row == row && enemy.col == col && enemy.isAlive) {
				return &enemy;
			}
		}
		return nullptr;
	}

	// 敵にダメージを与える（倒したら true）
	bool DamageEnemyAt(int row, int col, int damage)
	{
		EnemyData* enemy = FindEnemyAt(row, col);
		if (enemy != nullptr) {
			enemy->hp -= damage;
			if (enemy->hp <= 0) {
				enemy->hp = 0;
				enemy->isAlive = false;
				tiles[row][col] = ' ';
				return true;
			}
			return false;
		}
		return false;
	}

	// 戦闘なしで直接倒す場合用
	void DefeatEnemyAt(int row, int col)
	{
		EnemyData* enemy = FindEnemyAt(row, col);
		if (enemy != nullptr) {
			enemy->isAlive = false;
			enemy->hp = 0;
			tiles[row][col] = ' ';
		}
	}

	// 敵のHPをリセットする
	void ResetEnemyHP(int row, int col)
	{
		EnemyData* enemy = FindEnemyAt(row, col);
		if (enemy != nullptr) {
			enemy->hp = enemy->maxHp;
		}
	}

	// 生存している敵の数を返す
	int GetAliveEnemyCount() const
	{
		int count = 0;
		for (const auto& enemy : enemies) {
			if (enemy.isAlive) {
				count++;
			}
		}
		return count;
	}

	// 表示用データを作る（生存している敵を重ねる）。実際に描くのは MapView
	void BuildDisplayGrid(char (&out)[MAP_SIZE][MAP_SIZE]) const
	{
		for (int i = 0; i < MAP_SIZE; ++i) {
			for (int j = 0; j < MAP_SIZE; ++j) {
				out[i][j] = tiles[i][j];
			}
		}
		for (const auto& enemy : enemies) {
			if (enemy.isAlive) {
				out[enemy.row][enemy.col] = 'E';
			}
		}
	}

private:
	char tiles[MAP_SIZE][MAP_SIZE];
	std::vector<EnemyData> enemies;
};
