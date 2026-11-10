#pragma once
#include <vector>

// =============================================================
//  Model : ゲームの「状態」と「ルール」だけを持つ層
//  ここには cout も キー入力も Sleep も書かない（画面のことは知らない）
// =============================================================

// キャラクターの種類（以前は名前の文字列で判定していた）
enum class CharType { Normal, Soldier, Healer, Ranger };

struct PlayerSettings
{
	char Name[256];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPD;
	CharType Type = CharType::Normal;
};

struct EnemySettings
{
	char Name[256];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPD;
};

struct BossSettings
{
	char Name[256];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPD;
};

extern PlayerSettings hero;
extern PlayerSettings soldier;
extern PlayerSettings healer;
extern PlayerSettings ranger;
extern EnemySettings slime;
extern BossSettings dragon;

class Player
{
public:
	PlayerSettings Settings;

	Player(const Player& other)
		: Player(other.Settings)
	{
	}

	Player(const PlayerSettings& settings)
		: Settings(settings)
	{
	}
};

class Enemy
{
public:
	EnemySettings Settings;

	Enemy(const Enemy& other)
		: Enemy(other.Settings)
	{
	}

	Enemy(const EnemySettings& settings)
		: Settings(settings)
	{
	}
};

class Boss
{
public:
	BossSettings Settings;

	Boss(const Boss& other)
		: Boss(other.Settings)
	{
	}

	Boss(const BossSettings& settings)
		: Settings(settings)
	{
	}
};

// 仲間管理クラス（表示は StatusView::ShowParty に移した）
class PartyManager
{
private:
	std::vector<Player> allies;

public:
	PartyManager() {}

	void AddAlly(const Player& ally)
	{
		allies.push_back(ally);
	}

	const std::vector<Player>& GetAllies() const
	{
		return allies;
	}

	int GetAllyCount() const
	{
		return static_cast<int>(allies.size());
	}

	void ClearAllies()
	{
		allies.clear();
	}
};
