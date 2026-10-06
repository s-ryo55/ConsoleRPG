#pragma once
#include "Status.h"
#define NOMINMAX
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <limits>

using namespace std;

class Battle
{

public:
	void StartBattle(Player& player, Enemy& enemy) {
		// シードは初回だけで十分
		static bool seeded = false;
		if (!seeded) {
			std::srand(static_cast<unsigned>(std::time(nullptr)));
			seeded = true;
		}

		cout << "=== Battle Start! ===" << endl;
		// シンプルなターン制バトル
		bool playerDefending = false;

		// 表示用の遅延関数
		auto pause = [](int ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		};

		while (player.Settings.HP > 0 && enemy.Settings.HP > 0) {
			// ステータス表示
			cout << "------------------------" << endl;
			cout << "Player: " << player.Settings.Name
				<< "  HP:" << player.Settings.HP
				<< "  ATK:" << player.Settings.ATK
				<< "  DEF:" << player.Settings.DEF << endl;
			cout << "Enemy : " << enemy.Settings.Name
				<< "  HP:" << enemy.Settings.HP
				<< "  ATK:" << enemy.Settings.ATK
				<< "  DEF:" << enemy.Settings.DEF << endl;
			cout << "------------------------" << endl;

			// プレイヤーの選択
			cout << "1: Attack  2: Defend  3: Run" << endl;
			cout << "Choose: ";
			int choice = 0;
			if (!(cin >> choice)) {
				// 入力が不正ならクリアして再入力を促す
				cin.clear();
				cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
				cout << "無効な入力です。" << endl;
				continue;
			}

			if (choice == 1) {
				// 攻撃
				int base = player.Settings.ATK - (enemy.Settings.DEF / 2);
				if (base < 1) base = 1;
				// クリティカル 10%
				bool crit = (std::rand() % 100) < 10;
				int dmg = crit ? base * 2 : base;
				enemy.Settings.HP -= dmg;
				cout << "You attack for " << dmg << " damage" << (crit ? " (Critical!)" : "") << "." << endl;
				if (enemy.Settings.HP <= 0) {
					enemy.Settings.HP = 0;
					cout << enemy.Settings.Name << " を倒した！" << endl;
					break;
				}
			}
			else if (choice == 2) {
				// 防御：次の敵の攻撃ダメージを軽減
				playerDefending = true;
				cout << "You brace for the next attack." << endl;
			}
			else if (choice == 3) {
				// 逃走判定（50%）
				int r = std::rand() % 100;
				if (r < 50) {
					cout << "逃げ切った！" << endl;
					// 逃げに成功した場合は戦闘を終了（敵は生存）
					break;
				}
				else {
					cout << "逃走に失敗した！敵の攻撃を受ける！" << endl;
					// 失敗したら敵のターンに進む（追加のペナルティは無し）
				}
			}
			else {
				cout << "無効な選択です。" << endl;
				continue;
			}

			pause(500);

			// 敵ターン
			if (enemy.Settings.HP > 0) {
				// 敵の行動は単純に攻撃
				int base = enemy.Settings.ATK - (player.Settings.DEF / 2);
				if (base < 1) base = 1;
				// 防御時はダメージ半減
				int dmg = playerDefending ? (base + 1) / 2 : base;
				player.Settings.HP -= dmg;
				cout << enemy.Settings.Name << " attacks for " << dmg << " damage." << endl;
				if (player.Settings.HP <= 0) {
					player.Settings.HP = 0;
					cout << "あなたは倒れた..." << endl;
					break;
				}
			}

			// 防御フラグは敵のターンを終えたらリセット
			playerDefending = false;

			pause(500);
		}

		// 戦闘終了の表示
		cout << "=== Battle End ===" << endl;
		if (player.Settings.HP <= 0) {
			cout << "敗北..." << endl;
		}
		else if (enemy.Settings.HP <= 0) {
			cout << "勝利！" << endl;
		}
		else {
			cout << "戦闘から離脱しました。" << endl;
		}

		// 一応改行と短い待ち
		cout << endl;
		pause(800);
	}

};
