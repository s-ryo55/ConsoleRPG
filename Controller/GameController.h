#pragma once
#include <cstring>
#include "../Model/Status.h"
#include "../Model/Map.h"
#include "../Model/Move.h"
#include "../View/ConsoleView.h"
#include "../View/MapView.h"
#include "../View/StatusView.h"
#include "../View/GameView.h"
#include "BattleController.h"

// =============================================================
//  Controller : ゲーム全体の進行役（シーン遷移・キー入力）
//  Model（Map / Move / Player ...）を動かし、View に表示を頼む。
//  以前の main() に全部入っていたものを、役割ごとの関数に分けた。
// =============================================================
class GameController
{
public:
	void Run()
	{
		while (true) {
			HandleGlobalKeys();
			UpdateScene();
			ConsoleView::Pause(50);
		}
	}

private:
	enum Scene
	{
		TITLE,
		MENU,
		GAME,
		BATTLE,
		BOSS_BATTLE,
		WALL_BATTLE,
		GAMEOVER
	};

	// キーを「押した瞬間」だけ検出する小さな部品
	struct KeyLatch
	{
		bool held = false;
		bool Pressed(bool down)
		{
			if (down && !held) {
				held = true;
				return true;
			}
			if (!down) held = false;
			return false;
		}
	};

	static bool IsKeyDown(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

	// ---------- Model（ゲームの状態） ----------
	Map map_;
	Move playerMove_ = Move(1, 1);
	Player player_ = Player(hero);
	Enemy enemy_ = Enemy(slime);
	Boss boss_ = Boss(dragon);
	PartyManager party_;

	BattleController battle_;

	// ---------- 進行状態 ----------
	Scene scene_ = TITLE;
	bool needRedraw_ = true;  // 以前の display
	int selectMax_ = 0;
	int nowSelect_ = 0;

	KeyLatch tabKey_, spaceKey_, wKey_, aKey_, sKey_, dKey_;

	int battleEnemyRow_ = -1;
	int battleEnemyCol_ = -1;
	int battleWallRow_ = -1;
	int battleWallCol_ = -1;

	// ---------- 入力 ----------
	void HandleGlobalKeys()
	{
		if (tabKey_.Pressed(IsKeyDown(VK_TAB))) {
			needRedraw_ = true;
			scene_ = static_cast<Scene>(static_cast<int>(scene_) + 1);
			if (scene_ > GAMEOVER) {
				scene_ = TITLE;
			}
		}

		if (spaceKey_.Pressed(IsKeyDown(VK_SPACE)) && selectMax_ > 0) {
			nowSelect_ = (nowSelect_ + 1) % selectMax_;
			GameView::ShowSelect(nowSelect_);
		}
	}

	void UpdateScene()
	{
		switch (scene_) {
		case TITLE:       UpdateTitle();      break;
		case MENU:        UpdateMenu();       break;
		case GAME:        UpdateGame();       break;
		case BATTLE:      UpdateBattle();     break;
		case BOSS_BATTLE: UpdateBossBattle(); break;
		case WALL_BATTLE: UpdateWallBattle(); break;
		case GAMEOVER:    UpdateGameOver();   break;
		}
	}

	void ChangeScene(Scene next)
	{
		scene_ = next;
		needRedraw_ = true;
	}

	// ---------- 各シーン ----------
	void UpdateTitle()
	{
		if (!needRedraw_) return;
		ConsoleView::Clear();
		GameView::ShowTitle();
		GameView::ShowSelect(nowSelect_);
		selectMax_ = 1;
		needRedraw_ = false;
	}

	void UpdateMenu()
	{
		if (!needRedraw_) return;
		nowSelect_ = 0;  // メニューに入るたび選択位置を初期化
		ConsoleView::Clear();
		GameView::ShowMenu();
		std::cout << std::endl;
		GameView::ShowSelect(nowSelect_);

		selectMax_ = 2;
		needRedraw_ = false;

		if (nowSelect_ == 0) {
			scene_ = GAME;   // ゲームプレイに進む
		}
		else if (nowSelect_ == 1) {
			scene_ = TITLE;  // タイトルに戻る
		}
	}

	void UpdateGame()
	{
		if (needRedraw_) {
			ConsoleView::Clear();
			GameView::ShowGame();
			playerMove_.PlacePlayer(map_);
			MapView::Show(map_);
			StatusView::ShowParty(party_);
			GameView::ShowSelect(nowSelect_);
			needRedraw_ = false;
		}

		HandleMovement(IsKeyDown('W'), wKey_, 'w');
		HandleMovement(IsKeyDown('A'), aKey_, 'a');
		HandleMovement(IsKeyDown('S'), sKey_, 's');
		HandleMovement(IsKeyDown('D'), dKey_, 'd');
	}

	void RedrawMap()
	{
		ConsoleView::Clear();
		MapView::Show(map_);
		StatusView::ShowParty(party_);
		needRedraw_ = false;
	}

	void HandleMovement(bool keyDown, KeyLatch& latch, char direction)
	{
		if (!latch.Pressed(keyDown)) return;

		MoveResult result = playerMove_.MovePlayer(direction, map_);

		if (result != MoveResult::Moved) {
			if (playerMove_.HasWallEncounter()) {
				GameView::ShowWallEncounter();
			}
			GameView::ShowBlocked(playerMove_.GetInvalidStreak());
		}

		// 壁に50回ぶつかった → 壁との戦闘
		if (playerMove_.HasWallEncounter()) {
			battleWallRow_ = playerMove_.GetWallRow();
			battleWallCol_ = playerMove_.GetWallCol();
			playerMove_.ClearWallEncounter();
			ChangeScene(WALL_BATTLE);
			return;
		}

		if (result != MoveResult::Moved) return;

		if (playerMove_.HasAllyEncounter()) {
			OnAllyEncounter(playerMove_.GetEncounterType());
		}
		else if (playerMove_.HasBossEncounter()) {
			ChangeScene(BOSS_BATTLE);
			playerMove_.ClearEncounter();
		}
		else if (playerMove_.HasEncounter()) {
			// 敵の位置を記録
			battleEnemyRow_ = playerMove_.GetPlayerRow();
			battleEnemyCol_ = playerMove_.GetPlayerCol();
			ChangeScene(BATTLE);
			playerMove_.ClearEncounter();
		}
		else {
			RedrawMap();
		}
	}

	void OnAllyEncounter(char allyType)
	{
		if (allyType == 'S') {
			party_.AddAlly(Player(soldier));
			GameView::ShowAllyJoined(soldier.Name);
		}
		else if (allyType == 'H') {
			party_.AddAlly(Player(healer));
			GameView::ShowAllyJoined(healer.Name);
		}
		else if (allyType == 'R') {
			party_.AddAlly(Player(ranger));
			GameView::ShowAllyJoined(ranger.Name);
		}
		playerMove_.ClearEncounter();
		ConsoleView::Pause(800);
		RedrawMap();
	}

	void UpdateBattle()
	{
		if (!needRedraw_) return;

		ConsoleView::Clear();
		GameView::ShowBattle();

		EnemyData* target = map_.FindEnemyAt(battleEnemyRow_, battleEnemyCol_);
		if (target == nullptr) {
			GameView::ShowEnemyNotFound();
			ConsoleView::Pause(1000);
			ChangeScene(GAME);
			return;
		}

		GameView::ShowEnemyAppeared(slime.Name, target->hp, target->maxHp);

		// 戦闘は BattleController にお任せ（仲間も一緒に参加）
		battle_.StartBattle(player_, enemy_, party_.GetAllies());

		// 戦闘後の判定
		if (enemy_.Settings.HP <= 0) {
			map_.DamageEnemyAt(battleEnemyRow_, battleEnemyCol_, enemy_.Settings.ATK * 10);
			GameView::ShowEnemyDefeatedBackToMap();
			ConsoleView::Pause(1500);

			if (map_.GetAliveEnemyCount() == 0) {
				ConsoleView::Clear();
				GameView::ShowAllEnemiesDefeated();
				ChangeScene(GAMEOVER);
			}
			else {
				ChangeScene(GAME);
			}
		}
		else if (player_.Settings.HP <= 0) {
			GameView::ShowGameOverMessage();
			ConsoleView::Pause(1500);
			ChangeScene(GAMEOVER);
		}
		else {
			GameView::ShowEscapedBackToMap();
			ConsoleView::Pause(1500);
			ChangeScene(GAME);
		}

		// 敵HPをリセット
		enemy_.Settings.HP = slime.HP;
	}

	void UpdateBossBattle()
	{
		if (!needRedraw_) return;

		ConsoleView::Clear();
		GameView::ShowBossBattle();

		// 戦闘は Enemy 型で行うので、ボスの数値を Enemy にコピーして渡す
		// （以前は reinterpret_cast<Enemy&>(boss) で無理やり渡していた）
		Enemy bossAsEnemy(ToEnemySettings(boss_.Settings));
		battle_.StartBattle(player_, bossAsEnemy, party_.GetAllies());
		boss_.Settings.HP = bossAsEnemy.Settings.HP;

		if (boss_.Settings.HP <= 0) {
			// ボスをマップから消す
			map_.ReplaceTiles('B', ' ');
			playerMove_.PlacePlayer(map_);

			GameView::ShowBossDefeated();
			ChangeScene(GAMEOVER);
		}
		else if (player_.Settings.HP <= 0) {
			GameView::ShowGameOverMessage();
			ConsoleView::Pause(1500);
			ChangeScene(GAMEOVER);
		}
		else {
			GameView::ShowEscapedBackToMap();
			ConsoleView::Pause(1500);
			ChangeScene(GAME);
		}

		// ボスHPをリセット
		boss_.Settings.HP = dragon.HP;
	}

	void UpdateWallBattle()
	{
		if (!needRedraw_) return;

		ConsoleView::Clear();
		GameView::ShowBattle();

		EnemySettings wallSettings = { "壁", 1000, 0, 0, 0, 0 };
		GameView::ShowWallBattleStart(wallSettings.HP);

		Enemy wall(wallSettings);
		battle_.StartBattle(player_, wall, party_.GetAllies());

		if (wall.Settings.HP <= 0) {
			map_.SetTile(battleWallRow_, battleWallCol_, ' ');
			GameView::ShowWallDefeated();
			ConsoleView::Pause(1500);
			ChangeScene(GAME);
		}
		else if (player_.Settings.HP <= 0) {
			GameView::ShowGameOverMessage();
			ConsoleView::Pause(1500);
			ChangeScene(GAMEOVER);
		}
		else {
			GameView::ShowWallEscaped();
			ConsoleView::Pause(1000);
			ChangeScene(GAME);
		}
	}

	void UpdateGameOver()
	{
		if (!needRedraw_) return;
		ConsoleView::Clear();
		GameView::ShowGameOver();
		GameView::ShowSelect(nowSelect_);
		needRedraw_ = false;
	}

	// BossSettings と EnemySettings は同じ並びのデータなので、そのままコピーできる
	static EnemySettings ToEnemySettings(const BossSettings& b)
	{
		static_assert(sizeof(EnemySettings) == sizeof(BossSettings), "layout differs");
		EnemySettings e;
		std::memcpy(&e, &b, sizeof(e));
		return e;
	}
};
