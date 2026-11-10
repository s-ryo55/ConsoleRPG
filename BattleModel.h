#pragma once
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Status.h"

// =============================================================
//  Model : 戦闘のルール（ダメージ計算・防御・回復・逃走・勝敗）
//
//  ★ ここには cout も _getch も Sleep も書かない。
//     「何が起きたか」を結果の構造体で返すだけ。
//     それをどう見せるか → BattleView
//     どの順番で呼ぶか   → BattleController
//  画面を出さなくてもダメージ計算をテストできるのが、分けた利点。
// =============================================================

// 攻撃1回の結果
struct AttackResult
{
	int  damage = 0;
	bool critical = false;      // クリティカルか
	bool rangerBoost = false;   // レンジャーの必殺技（3倍）か
	bool enemyDefeated = false; // これで敵を倒したか
};

// 回復の結果
struct HealResult
{
	bool healed = false;
	int  targetIndex = -1;
	int  amount = 0;
};

// 敵 → プレイヤーへの攻撃結果
struct EnemyHitPlayer
{
	int  damage = 0;
	bool playerDown = false;
};

// 敵 → 仲間への攻撃結果
struct EnemyHitAlly
{
	bool dodged = false;  // ソルジャーが攻撃を回避した
	int  damage = 0;
	bool down = false;
};

enum class BattleOutcome { Victory, Defeat, Escaped };

class BattleModel
{
public:
	BattleModel(Player& player, Enemy& enemy, const std::vector<Player>& allies)
		: player_(player)
		, enemy_(enemy)
		, playerDefending_(false)
		, allyDefending_(allies.size(), false)
	{
		// 仲間のHPはコピーして扱う（戦闘後に元のデータへは反映されない：従来どおり）
		for (const auto& ally : allies) {
			allies_.push_back(ally.Settings);
		}

		// シードは初回だけで十分
		static bool seeded = false;
		if (!seeded) {
			std::srand(static_cast<unsigned>(std::time(nullptr)));
			seeded = true;
		}
	}

	// ---- 状態の参照（View が表示に使う） ----
	const PlayerSettings& GetPlayer() const { return player_.Settings; }
	const EnemySettings& GetEnemy() const { return enemy_.Settings; }
	const std::vector<PlayerSettings>& GetAllies() const { return allies_; }

	bool IsPlayerAlive() const { return player_.Settings.HP > 0; }
	bool IsEnemyAlive() const { return enemy_.Settings.HP > 0; }
	bool IsAllyAlive(size_t i) const { return allies_[i].HP > 0; }

	bool IsPartyAlive() const
	{
		if (player_.Settings.HP > 0) return true;
		for (const auto& ally : allies_) {
			if (ally.HP > 0) return true;
		}
		return false;
	}

	BattleOutcome GetOutcome() const
	{
		if (player_.Settings.HP <= 0) return BattleOutcome::Defeat;
		if (enemy_.Settings.HP <= 0) return BattleOutcome::Victory;
		return BattleOutcome::Escaped;
	}

	// ---- プレイヤーの行動 ----
	AttackResult PlayerAttack()
	{
		AttackResult r;
		int base = CalcBase(player_.Settings.ATK, enemy_.Settings.DEF);
		r.critical = Roll(10);  // クリティカル 10%
		r.damage = r.critical ? base * 2 : base;
		ApplyDamageToEnemy(r);
		return r;
	}

	void PlayerDefend() { playerDefending_ = true; }

	// 逃走（50%）。成功したら true
	bool TryRun() { return Roll(50); }

	// ---- 仲間の行動 ----
	// allowRangerBoost: レンジャーの必殺技（50%で3倍）を許可するか
	AttackResult AllyAttack(size_t i, bool allowRangerBoost)
	{
		AttackResult r;
		const PlayerSettings& a = allies_[i];
		int base = CalcBase(a.ATK, enemy_.Settings.DEF);
		bool crit = Roll(10);

		if (allowRangerBoost && a.Type == CharType::Ranger && Roll(50)) {
			r.damage = base * 3;
			r.rangerBoost = true;
		}
		else {
			r.damage = crit ? base * 2 : base;
			r.critical = crit;
		}
		ApplyDamageToEnemy(r);
		return r;
	}

	void AllyDefend(size_t i) { allyDefending_[i] = true; }

	// ヒーラーの回復。味方で最初に見つかった「HPが減っている人」を回復する
	HealResult AllyHeal(size_t i)
	{
		HealResult r;
		const int healAmount = allies_[i].ATK * 2;
		const int maxHP = kHealMaxHP;  // (C++14: std::min に static constexpr を直接渡さない)
		for (size_t j = 0; j < allies_.size(); ++j) {
			if (j != i && allies_[j].HP > 0 && allies_[j].HP < maxHP) {
				int oldHP = allies_[j].HP;
				allies_[j].HP = std::min(allies_[j].HP + healAmount, maxHP);
				r.healed = true;
				r.targetIndex = static_cast<int>(j);
				r.amount = allies_[j].HP - oldHP;
				break;
			}
		}
		return r;
	}

	// ---- 敵の行動 ----
	EnemyHitPlayer EnemyAttackPlayer()
	{
		EnemyHitPlayer r;
		int base = CalcBase(enemy_.Settings.ATK, player_.Settings.DEF);
		r.damage = playerDefending_ ? (base + 1) / 2 : base;  // 防御中はダメージ半減
		player_.Settings.HP -= r.damage;
		if (player_.Settings.HP <= 0) {
			player_.Settings.HP = 0;
			r.playerDown = true;
		}
		return r;
	}

	EnemyHitAlly EnemyAttackAlly(size_t i)
	{
		EnemyHitAlly r;
		PlayerSettings& a = allies_[i];
		int base = CalcBase(enemy_.Settings.ATK, a.DEF);

		// ソルジャーは50%の確率で攻撃を回避する
		if (a.Type == CharType::Soldier && Roll(50)) {
			r.dodged = true;
			return r;
		}

		r.damage = allyDefending_[i] ? (base + 1) / 2 : base;
		a.HP -= r.damage;
		if (a.HP <= 0) {
			a.HP = 0;
			r.down = true;
		}
		return r;
	}

	// ターン終了：防御フラグをリセット
	void EndTurn()
	{
		playerDefending_ = false;
		std::fill(allyDefending_.begin(), allyDefending_.end(), false);
	}

private:
	static constexpr int kHealMaxHP = 700;  // 回復の上限（従来のヒーラーのデフォルトHP）

	// 基本ダメージ = 攻撃力 - 防御力/2（最低1）
	static int CalcBase(int atk, int def)
	{
		int base = atk - (def / 2);
		return base < 1 ? 1 : base;
	}

	// percent% の確率で true
	static bool Roll(int percent) { return (std::rand() % 100) < percent; }

	void ApplyDamageToEnemy(AttackResult& r)
	{
		enemy_.Settings.HP -= r.damage;
		if (enemy_.Settings.HP <= 0) {
			enemy_.Settings.HP = 0;
			r.enemyDefeated = true;
		}
	}

	Player& player_;
	Enemy& enemy_;
	bool playerDefending_;
	std::vector<PlayerSettings> allies_;
	std::vector<bool> allyDefending_;
};
