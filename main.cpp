#include "Controller/GameController.h"

// main は「ゲームを作って動かす」だけ。
// 状態は Model、表示は View、進行は Controller が受け持つ。
int main()
{
	GameController game;
	game.Run();
	return 0;
}
