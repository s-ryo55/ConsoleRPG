#pragma once
#include <iostream>

// =============================================================
//  View : タイトル・メニュー・マップ画面まわりの文章
//  （以前の GameManager クラスと、main.cpp に散らばっていた cout をここに集約）
//  ※ 文言は元のコードに合わせて調整してください
// =============================================================
class GameView
{
public:
	// ---- 画面の見出し（以前の GameManager::Instance().XXX()） ----
	static void ShowTitle()
	{
		ShowControlsHint();
		std::cout << "GAME TITLE" << std::endl << std::endl;
		std::cout << "0: Press TAB to start the game." << std::endl;
	}

	static void ShowMenu()
	{
		ShowControlsHint();
		std::cout << "MENU" << std::endl << std::endl;
		std::cout << "0: GAME PLAYING" << std::endl;
		std::cout << "1: BACK TO TITLE" << std::endl;
	}

	static void ShowGame()
	{
		ShowControlsHint();
		std::cout << "GAME PLAYING" << std::endl << std::endl;
	}

	static void ShowBattle()
	{
		ShowControlsHint();
		std::cout << "BATTLE" << std::endl << std::endl;
	}

	static void ShowBossBattle()
	{
		ShowControlsHint();
		std::cout << "BOSS BATTLE" << std::endl << std::endl;
	}

	static void ShowGameOver()
	{
		ShowControlsHint();
		std::cout << "GAME OVER" << std::endl << std::endl;
	}

	static void ShowSelect(int nowSelect)
	{
		std::cout << "now_select: " << nowSelect << std::endl;
	}

	// ---- マップ上のメッセージ ----
	// 移動できなかった時のメッセージ（連続回数に応じて文言が変わる）
	// ★ 元の Move.h にあった 48 個のメッセージをここに貼り替えてください
	static void ShowBlocked(int invalidStreak)
	{
		static const char* messages[] = {
			"無効な方向です。",
			"そこは行けません。",
			"まだ移動できません。",
			"本当に行けません！",
			"その方向には進めません。",
			"その先には進めません。",
			"そちらへは移動できません。",
			"その道は通れません。"
		};
		const int msgCount = sizeof(messages) / sizeof(messages[0]);
		std::cout << messages[(invalidStreak - 1) % msgCount] << std::endl;
	}

	static void ShowWallEncounter()
	{
		std::cout << "壁を50回ぶつけた！壁との戦闘開始！" << std::endl;
	}

	static void ShowAllyJoined(const char* name)
	{
		std::cout << name << "が仲間になった！" << std::endl;
	}

	// ---- 通常戦闘 ----
	static void ShowEnemyAppeared(const char* name, int hp, int maxHp)
	{
		std::cout << "敵 " << name << " が現れた！" << std::endl;
		std::cout << "敵のHP: " << hp << "/" << maxHp << std::endl << std::endl;
	}

	static void ShowEnemyDefeatedBackToMap()
	{
		std::cout << "敵を倒しました！マップに戻ります。" << std::endl;
	}

	static void ShowAllEnemiesDefeated()
	{
		std::cout << "全ての敵を倒しました！ゲームクリア！" << std::endl;
	}

	static void ShowGameOverMessage()
	{
		std::cout << "ゲームオーバー..." << std::endl;
	}

	static void ShowEscapedBackToMap()
	{
		std::cout << "戦闘から逃げ出しました！マップに戻ります。" << std::endl;
	}

	static void ShowEnemyNotFound()
	{
		std::cout << "敵が見つかりません。" << std::endl;
	}

	// ---- ボス戦 ----
	static void ShowBossDefeated()
	{
		std::cout << "ボスを倒しました！ゲームクリア！" << std::endl;
	}

	// ---- 壁戦 ----
	static void ShowWallBattleStart(int wallHp)
	{
		std::cout << "壁との戦闘！ 壁HP: " << wallHp << std::endl;
	}

	static void ShowWallDefeated()
	{
		std::cout << "壁を倒した！壁を壊して通れるようになった！" << std::endl;
	}

	static void ShowWallEscaped()
	{
		std::cout << "壁との戦闘から逃げた。" << std::endl;
	}

private:
	static void ShowControlsHint()
	{
		std::cout << "TAB 決定　SPACE 選択　" << std::endl << std::endl;
	}
};
