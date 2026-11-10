#pragma once
#include "Map.h"

#define PLAYER_SYMBOL 'P'

// =============================================================
//  Model : プレイヤーの位置と、遭遇（敵・ボス・仲間・壁）の状態
//  以前は「連続入力メッセージ」を cout で出していたが、
//  ここでは結果（MoveResult）と連続回数だけ返し、文章は GameView が決める。
// =============================================================

enum class MoveResult
{
	Moved,       // 移動できた
	Blocked,     // 壁・範囲外で移動できなかった
	InvalidKey   // w/a/s/d 以外
};

class Move
{
	int playerX;
	int playerY;
	int invalidStreak = 0;          // 連続で移動できなかった回数
	bool encounteredEnemy = false;  // 敵と遭遇したか
	bool encounteredBoss = false;   // ボスと遭遇したか
	bool encounteredWall = false;   // 壁戦闘が発生したか
	int wallHitCount = 0;           // 壁にぶつかった回数
	int wallRow = -1;
	int wallCol = -1;
	char encounterType = '\0';      // 遭遇タイプ（'E', 'B', 'S', 'H', 'R'）

public:
	Move() : playerX(0), playerY(0) {}
	Move(int x, int y) : playerX(x), playerY(y) {}

	// 現在位置にプレイヤーを配置する（初期表示用）
	void PlacePlayer(Map& map)
	{
		if (playerX >= 0 && playerX < MAP_SIZE && playerY >= 0 && playerY < MAP_SIZE) {
			map.SetTile(playerY, playerX, PLAYER_SYMBOL);
		}
	}

	MoveResult MovePlayer(char direction, Map& map)
	{
		int newX = playerX;
		int newY = playerY;
		switch (direction) {
		case 'w':
			newY--;
			break;
		case 's':
			newY++;
			break;
		case 'a':
			newX--;
			break;
		case 'd':
			newX++;
			break;
		default:
			invalidStreak++;
			return MoveResult::InvalidKey;
		}

		const bool inBounds = newX >= 0 && newX < MAP_SIZE && newY >= 0 && newY < MAP_SIZE;

		if (inBounds && map.GetTile(newY, newX) != '#') {
			invalidStreak = 0;

			// 移動先に何があるかを確認
			char targetChar = map.GetTile(newY, newX);

			if (targetChar == 'E') {
				encounteredEnemy = true;
				encounterType = 'E';
			}
			else if (targetChar == 'B') {
				encounteredBoss = true;
				encounterType = 'B';
			}
			else if (targetChar == 'S' || targetChar == 'H' || targetChar == 'R') {
				encounterType = targetChar;
			}

			// もともとプレイヤーがいた位置の表示を消す
			if (playerX >= 0 && playerX < MAP_SIZE && playerY >= 0 && playerY < MAP_SIZE &&
				map.GetTile(playerY, playerX) == PLAYER_SYMBOL) {
				map.SetTile(playerY, playerX, ' ');
			}

			playerX = newX;
			playerY = newY;
			map.SetTile(playerY, playerX, PLAYER_SYMBOL);
			return MoveResult::Moved;
		}

		// 壁にぶつかった回数を数え、50回目で壁との戦闘を開始
		if (inBounds && map.GetTile(newY, newX) == '#') {
			if (!encounteredWall) {
				wallHitCount++;
				if (wallHitCount >= 50) {
					encounteredWall = true;
					wallRow = newY;
					wallCol = newX;
					wallHitCount = 0;
				}
			}
		}
		invalidStreak++;
		return MoveResult::Blocked;
	}

	// 遭遇フラグを参照/クリアするためのアクセサ
	bool HasEncounter() const { return encounteredEnemy; }
	bool HasBossEncounter() const { return encounteredBoss; }
	bool HasWallEncounter() const { return encounteredWall; }
	int GetWallRow() const { return wallRow; }
	int GetWallCol() const { return wallCol; }
	void ClearWallEncounter() { encounteredWall = false; }
	char GetEncounterType() const { return encounterType; }
	int GetInvalidStreak() const { return invalidStreak; }

	bool HasAllyEncounter() const
	{
		return encounterType == 'S' || encounterType == 'H' || encounterType == 'R';
	}

	void ClearEncounter()
	{
		encounteredEnemy = false;
		encounteredBoss = false;
		encounterType = '\0';
	}

	int GetPlayerRow() const { return playerY; }
	int GetPlayerCol() const { return playerX; }
};
