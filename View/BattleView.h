#pragma once
#include <iostream>
#include "../Model/BattleModel.h"

// =============================================================
//  View : 戦闘画面の表示。数値の計算はしない。
//  Model が返した結果（AttackResult など）を文章にするだけ。
//  ※ 文言は元のコードに合わせて調整してください
// =============================================================
class BattleView
{
public:
	static void ShowBattleStart()
	{
		std::cout << "=== Battle Start! ===" << std::endl;
	}

	// ステータス一覧
	static void ShowStatus(const BattleModel& m)
	{
		const PlayerSettings& p = m.GetPlayer();
		const EnemySettings& e = m.GetEnemy();
		const std::vector<PlayerSettings>& allies = m.GetAllies();

		std::cout << "------------------------" << std::endl;
		std::cout << "Player: " << p.Name
			<< "  HP:" << p.HP
			<< "  ATK:" << p.ATK
			<< "  DEF:" << p.DEF << std::endl;

		for (size_t i = 0; i < allies.size(); ++i) {
			std::cout << "Ally " << i + 1 << ": " << allies[i].Name
				<< "  HP:" << allies[i].HP
				<< "  ATK:" << allies[i].ATK
				<< "  DEF:" << allies[i].DEF << std::endl;
		}

		std::cout << "Enemy : " << e.Name
			<< "  HP:" << e.HP
			<< "  ATK:" << e.ATK
			<< "  DEF:" << e.DEF << std::endl;
		std::cout << "------------------------" << std::endl;
	}

	// ---- 入力まわりの表示 ----
	static void ShowPlayerMenu()
	{
		std::cout << "[Player Turn]" << std::endl;
		std::cout << "1: Attack  2: Defend  3: Run" << std::endl;
	}

	static void ShowAllyMenu()
	{
		std::cout << "1: Attack  2: Defend  3: Heal" << std::endl;
	}

	static void ShowChoosePrompt(int maxChoice)
	{
		std::cout << "Choose (1-" << maxChoice << "): ";
	}

	static void ShowChoiceEcho(int choice)
	{
		std::cout << choice << "\nTABキーで決定してください。" << std::endl;
	}

	// ---- プレイヤーの行動 ----
	static void ShowPlayerAttack(const EnemySettings& enemy, const AttackResult& r)
	{
		std::cout << "You attack for " << r.damage << " damage"
			<< (r.critical ? " (Critical!)" : "") << "." << std::endl;
		if (r.enemyDefeated) {
			std::cout << enemy.Name << " を倒した！" << std::endl;
		}
	}

	static void ShowPlayerDefend()
	{
		std::cout << "You brace for the next attack." << std::endl;
	}

	static void ShowRunResult(bool success)
	{
		if (success) {
			std::cout << "逃げ切った！" << std::endl;
		}
		else {
			std::cout << "逃走に失敗した！敵の攻撃を受ける！" << std::endl;
		}
	}

	// ---- 仲間の行動 ----
	static void ShowAllyTurn(const PlayerSettings& ally)
	{
		std::cout << "\n[" << ally.Name << " のターン]" << std::endl;
	}

	static void ShowAllyAttack(const PlayerSettings& ally, const AttackResult& r)
	{
		if (r.rangerBoost) {
			std::cout << ally.Name << " の必殺技！" << std::endl;
		}
		std::cout << ally.Name << " attacks for " << r.damage << " damage"
			<< (r.critical ? " (Critical!)" : "")
			<< (r.rangerBoost ? " (Ranger Boost x3!)" : "") << "." << std::endl;
	}

	static void ShowAllyDefend(const PlayerSettings& ally)
	{
		std::cout << ally.Name << " braces for the next attack." << std::endl;
	}

	static void ShowHeal(const PlayerSettings& healer, const PlayerSettings& target, int amount)
	{
		std::cout << healer.Name << " は " << target.Name
			<< " を " << amount << " 回復した！" << std::endl;
	}

	static void ShowNoHealTarget(const PlayerSettings& healer)
	{
		std::cout << healer.Name << " は回復対象がいない。通常攻撃を行った。" << std::endl;
	}

	// ---- 敵の行動 ----
	static void ShowEnemyTurn(const EnemySettings& enemy)
	{
		std::cout << "\n[" << enemy.Name << " のターン]" << std::endl;
	}

	static void ShowEnemyAttackPlayer(const EnemySettings& enemy, const EnemyHitPlayer& hit)
	{
		std::cout << enemy.Name << " attacks Player for " << hit.damage << " damage." << std::endl;
	}

	static void ShowPlayerDown()
	{
		std::cout << "あなたは倒れた..." << std::endl;
	}

	static void ShowEnemyHitAlly(const EnemySettings& enemy, const PlayerSettings& ally, const EnemyHitAlly& hit)
	{
		if (hit.dodged) {
			std::cout << ally.Name << " は敵の攻撃を回避した！" << std::endl;
			return;
		}
		std::cout << enemy.Name << " attacks " << ally.Name
			<< " for " << hit.damage << " damage." << std::endl;
		if (hit.down) {
			std::cout << ally.Name << " は倒れた..." << std::endl;
		}
	}

	// ---- 終了 ----
	static void ShowPartyWiped()
	{
		std::cout << "パーティは全滅した..." << std::endl;
	}

	static void ShowBattleEnd(BattleOutcome outcome)
	{
		std::cout << "=== Battle End ===" << std::endl;
		switch (outcome) {
		case BattleOutcome::Defeat:
			std::cout << "敗北..." << std::endl;
			break;
		case BattleOutcome::Victory:
			std::cout << "勝利！" << std::endl;
			break;
		default:
			std::cout << "戦闘から離脱しました。" << std::endl;
			break;
		}
		std::cout << std::endl;
	}
};
