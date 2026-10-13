#include <vector>
#include <iostream>
#include "Status.h"
#include "Move.h"
#include "Battle.h"
#include "Map.h"
#include <windows.h>

extern PlayerSettings hero;
extern PlayerSettings soldier;
extern PlayerSettings healer;
extern PlayerSettings ranger;
extern EnemySettings slime;
extern BossSettings dragon;

using namespace std;

class GameManager {
private:
	GameManager() {}
public:
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	static GameManager& Instance()
	{
		cout << "TAB 決定　　SPASE 選択　" << endl << endl;
		static GameManager instance;
		return instance;
	}

	void TitleGame()
	{
		cout << "GAME TITLE" << endl << endl;
		cout << "0: Press TAB to start the game." << endl;
	}

	void Menu()
	{
		cout << "MENU" << endl << endl;
		cout << "0: GAME PLAYING" << endl;
		cout << "1: BACK TO TITLE" << endl;
	}

	void Game()
	{
		cout << "GAME PLAYING" << endl << endl;
	}

	void Battle()
	{
		cout << "BATTLE" << endl << endl;
	}

	void BossBattle()
	{
		cout << "BOSS BATTLE" << endl << endl;
	}

	void GameOver()
	{
		cout << "GAME OVER" << endl << endl;
	}
};

void ClearConsole()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (h == INVALID_HANDLE_VALUE) return;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(h, &csbi)) return;

	DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	COORD homeCoords = { 0, 0 };
	DWORD count;

	FillConsoleOutputCharacterA(h, ' ', cellCount, homeCoords, &count);
	FillConsoleOutputAttribute(h, csbi.wAttributes, cellCount, homeCoords, &count);
	SetConsoleCursorPosition(h, homeCoords);
}

int main()
{
	Map map;
	Move playerMove(1, 1);
	Player player(hero);
	Enemy enemy(slime);
	Boss boss(dragon);
	Battle battle;
	PartyManager party;

	enum scenenum
	{
		TITLE,
		MENU,
		GAME,
		BATTLE,
		BOSS_BATTLE,
		GAMEOVER
	};

	int display = 1;
	int now_push_tab = 0;
	int now_push_space = 0;
	int now_push_w = 0;
	int now_push_a = 0;
	int now_push_s = 0;
	int now_push_d = 0;

	int select_max = 0;
	int select = 0;
	int now_select = 0;

	scenenum currentScene = TITLE;
	int battleEnemyRow = -1;  // 戦闘中の敵の行位置
	int battleEnemyCol = -1;  // 戦闘中の敵の列位置

	while (true)
	{
		bool tabDown = (GetAsyncKeyState(VK_TAB) & 0x8000) != 0;
		bool spaceDown = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

		if (tabDown && now_push_tab == 0)
		{
			now_push_tab = 1;
			display = 1;
			currentScene = static_cast<scenenum>(static_cast<int>(currentScene) + 1);
			if (currentScene > GAMEOVER)
			{
				currentScene = TITLE;
			}
		}
		else
		{
			if (!tabDown)
			{
				now_push_tab = 0;
			}
		}

		if (spaceDown && now_push_space == 0)
		{
			now_push_space = 1;
			now_select = (now_select + 1) % select_max;
			cout << "now_select: " << now_select << endl;
		}
		else
		{
			if (!spaceDown)
			{
				now_push_space = 0;
			}
		}

		switch (currentScene)
		{
		case TITLE:
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().TitleGame();
				cout << "now_select: " << now_select << endl;
				select_max = 1;
				display = 0;
			}
			break;

		case MENU:
		{
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().Menu();
			
				cout << endl;
				cout << "now_select: " << now_select << endl;

				select_max = 2;
				display = 0;

				if (now_select == 0) {
					// ゲームプレイに進む
					currentScene = GAME;
			
				}
				else if (now_select == 1) {
					// タイトルに戻る
					currentScene = TITLE;
				}
				
			}
			break;
		}

		case GAME:
		{
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().Game();

				playerMove.PlacePlayer(map);
				map.Map_Display();
				party.ShowAllies();

				cout << "now_select: " << now_select << endl;
				display = 0;
			}

			bool wDown = (GetAsyncKeyState('W') & 0x8000) != 0;
			bool aDown = (GetAsyncKeyState('A') & 0x8000) != 0;
			bool sDown = (GetAsyncKeyState('S') & 0x8000) != 0;
			bool dDown = (GetAsyncKeyState('D') & 0x8000) != 0;

			auto HandleMovement = [&](bool keyDown, int& keyState, char direction) {
				if (keyDown && keyState == 0) {
					keyState = 1;
					bool moved = playerMove.MovePlayer(direction, map);
					if (moved) {
						if (playerMove.HasAllyEncounter()) {
							char allyType = playerMove.GetEncounterType();
							if (allyType == 'S') {
								party.AddAlly(Player(soldier));
								cout << "ソルジャーが仲間になった！" << endl;
							}
							else if (allyType == 'H') {
								party.AddAlly(Player(healer));
								cout << "ヒーラーが仲間になった！" << endl;
							}
							else if (allyType == 'R') {
								party.AddAlly(Player(ranger));
								cout << "レンジャーが仲間になった！" << endl;
							}
							playerMove.ClearEncounter();
							Sleep(800);
							ClearConsole();
							map.Map_Display();
							party.ShowAllies();
							display = 0;
						}
						else if (playerMove.HasBossEncounter()) {
							currentScene = BOSS_BATTLE;
							display = 1;
							playerMove.ClearEncounter();
						}
						else if (playerMove.HasEncounter()) {
							// 敵の位置を記録
							battleEnemyRow = playerMove.GetPlayerRow();
							battleEnemyCol = playerMove.GetPlayerCol();
							currentScene = BATTLE;
							display = 1;
							playerMove.ClearEncounter();
						}
						else {
							ClearConsole();
							map.Map_Display();
							party.ShowAllies();
							display = 0;
						}
					}
				}
				else if (!keyDown) {
					keyState = 0;
				}
			};

			HandleMovement(wDown, now_push_w, 'w');
			HandleMovement(aDown, now_push_a, 'a');
			HandleMovement(sDown, now_push_s, 's');
			HandleMovement(dDown, now_push_d, 'd');
			break;
		}

		case BATTLE:
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().Battle();

				// 敵を検索
				EnemyData* targetEnemy = map.FindEnemyAt(battleEnemyRow, battleEnemyCol);
				if (targetEnemy != nullptr) {
					cout << "敵 " << slime.Name << " が現れた！" << endl;
					cout << "敵のHP: " << targetEnemy->hp << "/" << targetEnemy->maxHp << endl << endl;

					// 戦闘前のプレイヤーHPを保存
					int playerHPBefore = player.Settings.HP;

					// 戦闘処理を呼ぶ（味方も一緒に参加）
					battle.StartBattle(player, enemy, party.GetAllies());
					
					// 戦闘後の判定
					if (enemy.Settings.HP <= 0) {
						// 敵を倒した場合
						map.DamageEnemyAt(battleEnemyRow, battleEnemyCol, enemy.Settings.ATK * 10);
						cout << "敵を倒しました！マップに戻ります。" << endl;
						Sleep(1500);
						
						// 敵が全滅したか確認
						if (map.GetAliveEnemyCount() == 0) {
							ClearConsole();
							cout << "全ての敵を倒しました！ゲームクリア！" << endl;
							currentScene = GAMEOVER;
							display = 1;
						}
						else {
							// 戦闘後はゲーム画面に戻す
							currentScene = GAME;
							display = 1;
						}
					}
					else if (player.Settings.HP <= 0) {
						// プレイヤーが倒された場合
						cout << "ゲームオーバー..." << endl;
						Sleep(1500);
						currentScene = GAMEOVER;
					 display = 1;
					}
					else {
						// 逃走に成功した場合
						cout << "戦闘から逃げ出しました！マップに戻ります。" << endl;
						Sleep(1500);
						currentScene = GAME;
						display = 1;
					}

					// 敵HPをリセット
					enemy.Settings.HP = slime.HP;
				}
				else {
					// 敵が見つからない場合
					cout << "敵が見つかりません。" << endl;
					Sleep(1000);
					currentScene = GAME;
					display = 1;
				}
			}
			break;

		case BOSS_BATTLE:
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().BossBattle();

				// ボス戦も味方を一緒に参加させる
				battle.StartBattle(player, reinterpret_cast<Enemy&>(boss), party.GetAllies());

				// ボス戦後の判定
				if (boss.Settings.HP <= 0) {
					// ボスを倒した場合
					// ボスを削除
					for (int i = 0; i < MAP_SIZE; ++i) {
						for (int j = 0; j < MAP_SIZE; ++j) {
							if (map.map[i][j] == 'B') {
								map.map[i][j] = ' ';
							}
						}
					}

					playerMove.PlacePlayer(map);

					cout << "ボスを倒しました！ゲームクリア！" << endl;
					currentScene = GAMEOVER;
					display = 1;
				}
				else if (player.Settings.HP <= 0) {
					// プレイヤーが倒された場合
					cout << "ゲームオーバー..." << endl;
					Sleep(1500);
					currentScene = GAMEOVER;
					display = 1;
				}
				else {
					// 逃走に成功した場合
					cout << "戦闘から逃げ出しました！マップに戻ります。" << endl;
					Sleep(1500);
					currentScene = GAME;
					display = 1;
				}

				// ボスのHPをリセット
				boss.Settings.HP = dragon.HP;
			}
			break;

		case GAMEOVER:
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().GameOver();
				cout << "now_select: " << now_select << endl;
				display = 0;
			}
			break;
		}

		Sleep(50);
	}

	return 0;
}