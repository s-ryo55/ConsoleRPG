#include <iostream>
#include <windows.h>
#include"Status.h"
#include"Move.h"
#include"Battle.h"


using namespace std;

class GameManager {
private:
	GameManager() {}
public:
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	static GameManager& Instance()
	{
		cout << "TAB 決定　　SPASE 選択　" << endl<<endl;
		static GameManager instance;
		return instance;
	}

	void TitleGame()
	{
		
		cout << "GAME TITLE" << endl<<endl;
		cout << "0: Press TAB to start the game." << endl;
	}

	void Menu()
	{
		cout << "MENU" << endl<<endl;
		cout << "0: GAME PLAYING" << endl;	
		cout << "1: BACK TO TITLE" << endl;

	}

	void Game()
	{
		cout << "GAME PLAYING" << endl<<endl;
	}

	void Battle()
	{
		cout << "BATTLE" << endl<<endl;
	}

	void GameOver()
	{
		cout << "GAME OVER" << endl<<endl;

	}
};

// コンソール全体をクリアする関数
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

extern PlayerSettings hero; // 既に Status.h で extern 宣言されている
extern EnemySettings slime; // 既に Status.h で extern 宣言されている

int main()
{
	Map map;
	Move playerMove(1, 1);
	Player Player(hero); // Player クラスのインスタンスを作成
	Enemy enemy(slime); // Enemy クラスのインスタンスを作成
	Battle battle; // 追加：戦闘処理オブジェクト


	enum scenenum
	{
		TITLE,
		MENU,
		GAME,
		BATTLE,
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

	while (true)
	{
		// 高ビットで押下中か判定する
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
				ClearConsole(); // 押すたびに前の文字を消す
				GameManager::Instance().TitleGame();
				cout << "now_select: " << now_select << endl;
				select_max = 1;
				display = 0;
			}
			break;
		case MENU:
			

			if (display == 1)
			{
				if (now_select == 0) {
					ClearConsole();
					GameManager::Instance().Menu();
					cout << "now_select: " << now_select << endl;

					select_max = 2;
					display = 0;
				}else if (now_select == 1) {
					ClearConsole();
					GameManager::Instance().Menu();
					cout << "now_select: " << now_select << endl;
					select_max = 2;
					display = 0;
				}
			}
			break;

		case GAME:
		{
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().Game();

				// プレイヤーがマップに配置されていることを確実にする
				playerMove.PlacePlayer(map);
				map.Map_Display();

				cout << "now_select: " << now_select << endl;
				display = 0;
			}

			// WASD キーの状態を取得（大文字の VK コードを使用）
			bool wDown = (GetAsyncKeyState('W') & 0x8000) != 0;
			bool aDown = (GetAsyncKeyState('A') & 0x8000) != 0;
			bool sDown = (GetAsyncKeyState('S') & 0x8000) != 0;
			bool dDown = (GetAsyncKeyState('D') & 0x8000) != 0;

			// W
			if (wDown && now_push_w == 0) {
				now_push_w = 1;
				bool moved = playerMove.MovePlayer('w', map);
				if (moved) {
					// 敵に遭遇したらバトルへ遷移
					if (playerMove.HasEncounter()) {
						currentScene = BATTLE;
						display = 1;
						playerMove.ClearEncounter();
					}
					else {
						ClearConsole();
						map.Map_Display();
						display = 0;
					}
				}
			}
			else if (!wDown) {
				now_push_w = 0;
			}

			// A
			if (aDown && now_push_a == 0) {
				now_push_a = 1;
				bool moved = playerMove.MovePlayer('a', map);
				if (moved) {
					if (playerMove.HasEncounter()) {
						currentScene = BATTLE;
						display = 1;
						playerMove.ClearEncounter();
					}
					else {
						ClearConsole();
						map.Map_Display();
						display = 0;
					}
				}
			}
			else if (!aDown) {
				now_push_a = 0;
			}

			// S
			if (sDown && now_push_s == 0) {
				now_push_s = 1;
				bool moved = playerMove.MovePlayer('s', map);
				if (moved) {
					if (playerMove.HasEncounter()) {
						currentScene = BATTLE;
						display = 1;
						playerMove.ClearEncounter();
					}
					else {
						ClearConsole();
						map.Map_Display();
						display = 0;
					}
				}
			}
			else if (!sDown) {
				now_push_s = 0;
			}

			// D
			if (dDown && now_push_d == 0) {
				now_push_d = 1;
				bool moved = playerMove.MovePlayer('d', map);
				if (moved) {
					if (playerMove.HasEncounter()) {
						currentScene = BATTLE;
						display = 1;
						playerMove.ClearEncounter();
					}
					else {
						ClearConsole();
						map.Map_Display();
						display = 0;
					}
				}
			}
			else if (!dDown) {
				now_push_d = 0;
			}
			break;
		}
		case BATTLE:
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().Battle();

				// 戦闘処理を呼ぶ
				battle.StartBattle(Player, enemy);

				// 戦闘後：マップ上の敵（'E'）を削除（敵を倒した想定）
				for (int i = 0; i < MAP_SIZE; ++i) {
					for (int j = 0; j < MAP_SIZE; ++j) {
						if (map.map[i][j] == 'E') {
							map.map[i][j] = ' ';
						}
					}
				}

				// プレイヤー位置を再描画したい場合は再配置（必要なら）
				playerMove.PlacePlayer(map);

				// 戦闘後はゲーム画面に戻す
				currentScene = GAME;
				display = 1; // 次ループで再描画
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
}

