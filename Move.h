#pragma once
#include <iostream>
#include "Map.h"
#define PLAYER_SYMBOL 'P'

class Move : public Map
{
	int playerX;
	int playerY;
	int invalidStreak = 0; // 連続無効入力回数を保持
	bool encounteredEnemy = false; // 敵と遭遇したか

public:

	Move() {}
	Move(int x, int y) : playerX(x), playerY(y) {}

	// 現在位置にプレイヤーを配置する（初期表示用）
	void PlacePlayer(Map& map) {
		if (playerX >= 0 && playerX < MAP_SIZE && playerY >= 0 && playerY < MAP_SIZE) {
			map.map[playerY][playerX] = PLAYER_SYMBOL;
		}
	}

	// 移動に成功したら true、失敗したら false を返す
	bool MovePlayer(char direction, Map& map) {
		static const char* messages[] = {
			"無効な方向です。",
"そこは行けません。",
"まだ移動できません。",
"本当に行けません！",
"その方向には進めません。",
"その先には進めません。",
"そこへは移動できません。",
"その道は通れません。",
"その方向は選択できません。",
"そこには進めません。",
"この先には行けません。",
"これ以上は進めません。",
"その場所には行けません。",
"その方向は無効です。",
"移動先がありません。",
"進める道がありません。",
"その道は使えません。",
"その方向には道がありません。",
"そこへ向かうことはできません。",
"その先へは行けません。",
"今はそこへ行けません。",
"今は移動できません。",
"この方向には進めません。",
"そのルートは使えません。",
"進行できない方向です。",
"移動できない方向です。",
"その方向は選べません。",
"そこへ進むことはできません。",
"その先は通行できません。",
"この先は通れません。",
"その道は進めません。",
"その場所へは行けません。",
"進行先が不正です。",
"移動先が不正です。",
"指定された方向には進めません。",
"その方向への移動はできません。",
"その先への移動はできません。",
"現在はその方向へ進めません。",
"その方向には対応していません。",
"その移動は許可されていません。",
"その方向への移動は無効です。",
"そこへ移動することはできません。",
"進行方向を変更してください。",
"別の方向を選んでください。",
"正しい方向を選んでください。",
"進める方向を選んでください。",
"その方向では進めません！",
"そこへは行けません！",
"その先には進めません！",
"この方向はダメみたいです！"
		};
		const int msgCount = sizeof(messages) / sizeof(messages[0]);

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
			// 無効なキー入力は連続カウントを増やしてメッセージを変化させる
			invalidStreak++;
			std::cout << messages[(invalidStreak - 1) % msgCount] << std::endl;
			return false;
		}

		if (newX >= 0 && newX < MAP_SIZE && newY >= 0 && newY < MAP_SIZE && map.map[newY][newX] != '#') {
			// 成功したらカウントをリセット
			invalidStreak = 0;

			// もし移動先に敵 'E' がいたら遭遇フラグを立てる
			if (map.map[newY][newX] == 'E') {
				encounteredEnemy = true;
			}

			// もともとプレイヤーがいた位置の表示を消す（プレイヤー記号と一致している場合のみ）
			if (playerX >= 0 && playerX < MAP_SIZE && playerY >= 0 && playerY < MAP_SIZE &&
				map.map[playerY][playerX] == PLAYER_SYMBOL) {
				map.map[playerY][playerX] = ' ';
			}

			playerX = newX;
			playerY = newY;
			// 敵と遭遇した場合でもプレイヤー表示にする（メインでシーン遷移する）
			map.map[playerY][playerX] = PLAYER_SYMBOL;
			return true;
		}
		else {
			// 移動阻害（壁など）も連続無効扱いにしてメッセージを変える
			invalidStreak++;
			std::cout << messages[(invalidStreak - 1) % msgCount] << std::endl;
			return false;
		}
	}

	// 敵遭遇フラグを参照/クリアするためのアクセサ
	bool HasEncounter() const { return encounteredEnemy; }
	void ClearEncounter() { encounteredEnemy = false; }
};