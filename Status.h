#pragma once

struct PlayerSettings
{
	char Name[256];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPD;
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

extern PlayerSettings hero; // ƒwƒbƒ_‚Å‚Í extern éŒ¾‚É‚·‚é
extern EnemySettings slime; // “Gİ’è‚Ì extern éŒ¾‚ğ’Ç‰Á

class Player
{
public:
	void Show() const;

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
	void Show() const;
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