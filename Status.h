#pragma once
#include <vector>

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

struct BossSettings
{
	char Name[256];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int SPD;
};

extern PlayerSettings hero; // ヘッダでは extern 宣言にする
extern PlayerSettings soldier; // ソルジャー
extern PlayerSettings healer; // ヒーラー
extern PlayerSettings ranger; // レンジャー
extern EnemySettings slime; // 敵設定の extern 宣言を追加
extern BossSettings dragon; // ボス設定の extern 宣言を追加

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

class Boss
{
	public:
	void Show() const;
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

// 仲間管理クラス
class PartyManager
{
private:
	std::vector<Player> allies;

public:
	PartyManager() {}

	// 仲間を追加
	void AddAlly(const Player& ally) {
		allies.push_back(ally);
	}

	// 仲間一覧を取得
	const std::vector<Player>& GetAllies() const {
		return allies;
	}

	// 仲間の数を取得
	int GetAllyCount() const {
		return static_cast<int>(allies.size());
	}

	// 仲間情報を表示
	void ShowAllies() const {
		std::cout << "=== 仲間情報 ===" << std::endl;
		for (size_t i = 0; i < allies.size(); ++i) {
			std::cout << i + 1 << ". " << allies[i].Settings.Name
				<< " (HP: " << allies[i].Settings.HP
				<< ", ATK: " << allies[i].Settings.ATK
				<< ", DEF: " << allies[i].Settings.DEF << ")" << std::endl;
		}
		std::cout << std::endl;
	}

	// 仲間をクリア
	void ClearAllies() {
		allies.clear();
	}
};