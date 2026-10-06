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

extern PlayerSettings hero; // ƒwƒbƒ_‚Å‚Í extern éŒ¾‚É‚·‚é

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