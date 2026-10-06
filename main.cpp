#include <iostream>
#include <windows.h>
#include"Status.h"
#include"Move.h"

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
		
		cout << "GAME TITLE" << endl;
		cout << "0: Press TAB to start the game." << endl;
	}

	void Menu()
	{
		cout << "MENU" << endl;
		cout << "0: GAME PLAYING" << endl;	
		cout << "1: BACK TO TITLE" << endl;

	}

	void Game()
	{
		cout << "0: GAME PLAYING" << endl;
	}

	void GameOver()
	{
		cout << "0: GAME OVER" << endl;

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

int main()
{
	enum scenenum
	{
		TITLE,
		MENU,
		GAME,
		GAMEOVER
	};
	int display = 1;
	int now_push_tab = 0;
	int now_push_space = 0;

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
			if (display == 1)
			{
				ClearConsole();
				GameManager::Instance().Game();
				cout << "now_select: " << now_select << endl;
				display = 0;
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

