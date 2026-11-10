#pragma once
#include <conio.h>
#include <vector>
#include "../Model/BattleModel.h"
#include "../View/BattleView.h"
#include "../View/ConsoleView.h"

// =============================================================
//  Controller : 戦闘の進行役（店長）
//  入力を受け取り、Model に「やって」と頼み、View に「見せて」と頼む。
//  ダメージ計算そのものは書かない（書いた瞬間 Model と二重になる）。
// =============================================================
class BattleController
{
public:
	void StartBattle(Player& player, Enemy& enemy, const std::vector<Player>& allies = std::vector<Player>())
	{
		BattleModel model(player, enemy, allies);

		BattleView::ShowBattleStart();

		while (model.IsPlayerAlive() && model.IsEnemyAlive()) {
			BattleView::ShowStatus(model);

			// ---- プレイヤーのターン ----
			BattleView::ShowPlayerMenu();
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));  // 残っていたキー入力を捨てる
			BattleView::ShowChoosePrompt(3);
			int choice = ReadChoiceWithTab(3);

			if (choice == 1) {
				AttackResult r = model.PlayerAttack();
				BattleView::ShowPlayerAttack(model.GetEnemy(), r);
				if (r.enemyDefeated) break;
			}
			else if (choice == 2) {
				model.PlayerDefend();
				BattleView::ShowPlayerDefend();
			}
			else if (choice == 3) {
				bool escaped = model.TryRun();
				BattleView::ShowRunResult(escaped);
				if (escaped) break;
			}

			ConsoleView::Pause(500);

			// ---- 仲間のターン ----
			if (model.IsEnemyAlive()) {
				RunAllyTurns(model);
			}

			// ---- 敵のターン ----
			if (model.IsEnemyAlive()) {
				bool playerSurvived = RunEnemyTurn(model);
				if (!playerSurvived) break;
			}

			model.EndTurn();

			if (!model.IsPartyAlive()) {
				BattleView::ShowPartyWiped();
				break;
			}

			ConsoleView::Pause(500);
		}

		BattleView::ShowBattleEnd(model.GetOutcome());
		ConsoleView::Pause(800);
	}

private:
	// 数字キーで選び、TABキーで決定する（Enterは使わない）
	int ReadChoiceWithTab(int maxChoice)
	{
		while (true) {
			int key = _getch();
			if (key >= '1' && key <= '0' + maxChoice) {
				int choice = key - '0';
				BattleView::ShowChoiceEcho(choice);
				while (true) {
					int confirm = _getch();
					if (confirm == 9) return choice;  // TAB
					if (confirm == 27) break;         // ESC：選び直し
				}
				BattleView::ShowChoosePrompt(maxChoice);
			}
		}
	}

	void RunAllyTurns(BattleModel& model)
	{
		const std::vector<PlayerSettings>& allies = model.GetAllies();

		for (size_t i = 0; i < allies.size(); ++i) {
			if (!model.IsAllyAlive(i)) continue;

			const PlayerSettings& ally = allies[i];
			BattleView::ShowAllyTurn(ally);
			BattleView::ShowAllyMenu();
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			BattleView::ShowChoosePrompt(3);
			int choice = ReadChoiceWithTab(3);

			AttackResult attack;
			bool attacked = false;

			if (choice == 1) {
				attack = model.AllyAttack(i, true);  // レンジャーは必殺技あり
				attacked = true;
			}
			else if (choice == 2) {
				model.AllyDefend(i);
				BattleView::ShowAllyDefend(ally);
			}
			else {  // 3: ヒーラーは回復、それ以外は通常攻撃
				if (ally.Type == CharType::Healer) {
					HealResult heal = model.AllyHeal(i);
					if (heal.healed) {
						BattleView::ShowHeal(ally, allies[heal.targetIndex], heal.amount);
					}
					else {
						BattleView::ShowNoHealTarget(ally);
						attack = model.AllyAttack(i, false);
						attacked = true;
					}
				}
				else {
					attack = model.AllyAttack(i, false);
					attacked = true;
				}
			}

			if (attacked) {
				BattleView::ShowAllyAttack(ally, attack);
				if (attack.enemyDefeated) return;
			}

			ConsoleView::Pause(500);
		}
	}

	// プレイヤーが生き残ったら true
	bool RunEnemyTurn(BattleModel& model)
	{
		const EnemySettings& enemy = model.GetEnemy();

		BattleView::ShowEnemyTurn(enemy);
		ConsoleView::Pause(500);

		EnemyHitPlayer hitPlayer = model.EnemyAttackPlayer();
		BattleView::ShowEnemyAttackPlayer(enemy, hitPlayer);
		if (hitPlayer.playerDown) {
			BattleView::ShowPlayerDown();
			return false;
		}

		// 敵は仲間にも攻撃する
		const std::vector<PlayerSettings>& allies = model.GetAllies();
		for (size_t i = 0; i < allies.size(); ++i) {
			if (!model.IsAllyAlive(i)) continue;
			EnemyHitAlly hit = model.EnemyAttackAlly(i);
			BattleView::ShowEnemyHitAlly(enemy, allies[i], hit);
		}
		return true;
	}
};
