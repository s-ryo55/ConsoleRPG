#pragma once
#include "Status.h"
#define NOMINMAX
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <limits>
#include <vector>
#include <cstring>
#include <windows.h>
#include <conio.h>

using namespace std;

class Battle
{

private:
	// キャラクター名からキャラクタータイプを判定
	const char* GetCharacterType(const char* name) {
		if (strstr(name, "ソルジャー") != nullptr) {
			return "Soldier";
		}
		else if (strstr(name, "ヒーラー") != nullptr) {
			return "Healer";
		}
		else if (strstr(name, "レンジャー") != nullptr) {
			return "Ranger";
		}
		return "Normal";
	}

	// 数字キーで選択し、TABキーで決定する（Enterは使わない）
	int ReadChoiceWithTab(int maxChoice) {
		while (true) {
			int key = _getch();
			if (key >= '1' && key <= '0' + maxChoice) {
				int choice = key - '0';
				cout << choice << "\nTABキーで決定してください。" << endl;
				while (true) {
					int confirm = _getch();
					if (confirm == 9) return choice;
					if (confirm == 27) break;
				}
				cout << "Choose (1-" << maxChoice << "): ";
			}
		}
	}

public:
	void StartBattle(Player& player, Enemy& enemy, const vector<Player>& allies = vector<Player>()) {
		// シードは初回だけで十分
		static bool seeded = false;
		if (!seeded) {
			std::srand(static_cast<unsigned>(std::time(nullptr)));
			seeded = true;
		}

		cout << "=== Battle Start! ===" << endl;
		// シンプルなターン制バトル
		bool playerDefending = false;
		vector<bool> alliesDefending(allies.size(), false);

		// 表示用の遅延関数
		auto pause = [](int ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			};

		// 味方のHP情報をコピー
		vector<PlayerSettings> alliesSettings;
		for (const auto& ally : allies) {
			alliesSettings.push_back(ally.Settings);
		}

		while (player.Settings.HP > 0 && enemy.Settings.HP > 0) {
			// ステータス表示
			cout << "------------------------" << endl;
			cout << "Player: " << player.Settings.Name
				<< "  HP:" << player.Settings.HP
				<< "  ATK:" << player.Settings.ATK
				<< "  DEF:" << player.Settings.DEF << endl;

			// 味方のステータス表示
			for (size_t i = 0; i < alliesSettings.size(); ++i) {
				cout << "Ally " << i + 1 << ": " << alliesSettings[i].Name
					<< "  HP:" << alliesSettings[i].HP
					<< "  ATK:" << alliesSettings[i].ATK
					<< "  DEF:" << alliesSettings[i].DEF << endl;
			}

			cout << "Enemy : " << enemy.Settings.Name
				<< "  HP:" << enemy.Settings.HP
				<< "  ATK:" << enemy.Settings.ATK
				<< "  DEF:" << enemy.Settings.DEF << endl;
			cout << "------------------------" << endl;

			// プレイヤーの選択
			cout << "[Player Turn]" << endl;
			cout << "1: Attack  2: Defend  3: Run" << endl;
			// 探索中などに押されたキーが残っていても、Choose の入力に混ざらないようにする
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			cout << "Choose (1-3): ";
			int choice = ReadChoiceWithTab(3);

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

			// 味方のターン（敵がまだ生きている場合）
			if (enemy.Settings.HP > 0) {
				for (size_t i = 0; i < alliesSettings.size(); ++i) {
					if (alliesSettings[i].HP > 0) {
						cout << "\n[" << alliesSettings[i].Name << " のターン]" << endl;
						cout << "1: Attack  2: Defend  3: Heal" << endl;
						// 前の入力が味方の選択に残らないようにする
						FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
						cout << "Choose (1-3): ";
						int allyChoice = ReadChoiceWithTab(3);

						const char* charType = GetCharacterType(alliesSettings[i].Name);

						if (allyChoice == 1) {
							// 攻撃
							int base = alliesSettings[i].ATK - (enemy.Settings.DEF / 2);
							if (base < 1) base = 1;

							int dmg = base;
							bool crit = (std::rand() % 100) < 10;
							bool ranger_boost = false;

							// レンジャーは50%の確率で威力3倍
							if (strcmp(charType, "Ranger") == 0) {
								if ((std::rand() % 100) < 50) {
									dmg = base * 3;
									ranger_boost = true;
									cout << alliesSettings[i].Name << " の必殺技！" << endl;
								}
								else {
									dmg = crit ? base * 2 : base;
								}
							}
							else {
								dmg = crit ? base * 2 : base;
							}

							enemy.Settings.HP -= dmg;
							cout << alliesSettings[i].Name << " attacks for " << dmg << " damage"
								<< (crit && !ranger_boost ? " (Critical!)" : "")
								<< (ranger_boost ? " (Ranger Boost x3!)" : "") << "." << endl;
							if (enemy.Settings.HP <= 0) {
								enemy.Settings.HP = 0;
								break;
							}
						}
						else if (allyChoice == 2) {
							// 防御
							alliesDefending[i] = true;
							cout << alliesSettings[i].Name << " braces for the next attack." << endl;
						}
						else if (allyChoice == 3) {
							// ヒーラーは回復できる
							if (strcmp(charType, "Healer") == 0) {
								int healAmount = alliesSettings[i].ATK * 2;
								int maxHP = 700; // ヒーラーのデフォルトHP

								// 他のキャラを回復
								bool healed = false;
								for (size_t j = 0; j < alliesSettings.size(); ++j) {
									if (j != i && alliesSettings[j].HP > 0 && alliesSettings[j].HP < maxHP) {
										int oldHP = alliesSettings[j].HP;
										alliesSettings[j].HP = min(alliesSettings[j].HP + healAmount, maxHP);
										cout << alliesSettings[i].Name << " は " << alliesSettings[j].Name
											<< " を " << (alliesSettings[j].HP - oldHP) << " 回復した！" << endl;
										healed = true;
										break;
									}
								}
								if (!healed) {
									cout << alliesSettings[i].Name << " は回復対象がいない。通常攻撃を行った。" << endl;
									// 回復対象がない場合は攻撃
									int base = alliesSettings[i].ATK - (enemy.Settings.DEF / 2);
									if (base < 1) base = 1;
									bool crit = (std::rand() % 100) < 10;
									int dmg = crit ? base * 2 : base;
									enemy.Settings.HP -= dmg;
									cout << alliesSettings[i].Name << " attacks for " << dmg << " damage"
										<< (crit ? " (Critical!)" : "") << "." << endl;
									if (enemy.Settings.HP <= 0) {
										enemy.Settings.HP = 0;
										break;
									}
								}
							}
							else {
								// ヒーラー以外が3を選んだ場合は攻撃
								int base = alliesSettings[i].ATK - (enemy.Settings.DEF / 2);
								if (base < 1) base = 1;
								bool crit = (std::rand() % 100) < 10;
								int dmg = crit ? base * 2 : base;
								enemy.Settings.HP -= dmg;
								cout << alliesSettings[i].Name << " attacks for " << dmg << " damage"
									<< (crit ? " (Critical!)" : "") << "." << endl;
								if (enemy.Settings.HP <= 0) {
									enemy.Settings.HP = 0;
									break;
								}
							}
						}
						else {
							// 無効な選択は攻撃
							int base = alliesSettings[i].ATK - (enemy.Settings.DEF / 2);
							if (base < 1) base = 1;
							bool crit = (std::rand() % 100) < 10;
							int dmg = crit ? base * 2 : base;
							enemy.Settings.HP -= dmg;
							cout << alliesSettings[i].Name << " attacks for " << dmg << " damage"
								<< (crit ? " (Critical!)" : "") << "." << endl;
							if (enemy.Settings.HP <= 0) {
								enemy.Settings.HP = 0;
								break;
							}
						}

						pause(500);
					}
				}
			}

			// 敵がまだ生きている場合の敵ターン
			if (enemy.Settings.HP > 0) {
				cout << "\n[" << enemy.Settings.Name << " のターン]" << endl;
				pause(500);

				// 敵の行動は単純に攻撃
				int base = enemy.Settings.ATK - (player.Settings.DEF / 2);
				if (base < 1) base = 1;
				// 防御時はダメージ半減
				int dmg = playerDefending ? (base + 1) / 2 : base;
				player.Settings.HP -= dmg;
				cout << enemy.Settings.Name << " attacks Player for " << dmg << " damage." << endl;
				if (player.Settings.HP <= 0) {
					player.Settings.HP = 0;
					cout << "あなたは倒れた..." << endl;
					break;
				}

				// 敵が味方にも攻撃
				for (size_t i = 0; i < alliesSettings.size(); ++i) {
					if (alliesSettings[i].HP > 0) {
						const char* charType = GetCharacterType(alliesSettings[i].Name);
						int allyDef = alliesSettings[i].DEF;
						int allyBase = enemy.Settings.ATK - (allyDef / 2);
						if (allyBase < 1) allyBase = 1;

						// ソルジャーは50%の確率で攻撃を回避
						if (strcmp(charType, "Soldier") == 0 && (std::rand() % 100) < 50) {
							cout << alliesSettings[i].Name << " は敵の攻撃を回避した！" << endl;
							continue;
						}

						int allyDmg = alliesDefending[i] ? (allyBase + 1) / 2 : allyBase;
						alliesSettings[i].HP -= allyDmg;
						cout << enemy.Settings.Name << " attacks " << alliesSettings[i].Name
							<< " for " << allyDmg << " damage." << endl;
						if (alliesSettings[i].HP <= 0) {
							alliesSettings[i].HP = 0;
							cout << alliesSettings[i].Name << " は倒れた..." << endl;
						}
					}
				}
			}

			// 防御フラグはターンを終えたらリセット
			playerDefending = false;
			fill(alliesDefending.begin(), alliesDefending.end(), false);

			// パーティ全滅判定
			bool partyAlive = player.Settings.HP > 0;
			for (size_t i = 0; i < alliesSettings.size(); ++i) {
				if (alliesSettings[i].HP > 0) {
					partyAlive = true;
					break;
				}
			}

			if (!partyAlive) {
				cout << "パーティは全滅した..." << endl;
				break;
			}

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
