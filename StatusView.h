#pragma once
#include <iostream>
#include "../Model/Status.h"

// =============================================================
//  View : ステータス・仲間情報の表示
//  （以前は Player::Show / PartyManager::ShowAllies など Model の中にあった）
// =============================================================
class StatusView
{
public:
	// PlayerSettings / EnemySettings / BossSettings のどれでも表示できる
	template <typename Settings>
	static void ShowSettings(const Settings& s)
	{
		std::cout << "Name: " << s.Name
			<< ", HP: " << s.HP
			<< ", MP: " << s.MP
			<< ", ATK: " << s.ATK
			<< ", DEF: " << s.DEF
			<< ", SPD: " << s.SPD
			<< std::endl;
	}

	static void ShowParty(const PartyManager& party)
	{
		const std::vector<Player>& allies = party.GetAllies();

		std::cout << "=== 仲間情報 ===" << std::endl;
		for (size_t i = 0; i < allies.size(); ++i) {
			std::cout << i + 1 << ". " << allies[i].Settings.Name
				<< " (HP: " << allies[i].Settings.HP
				<< ", ATK: " << allies[i].Settings.ATK
				<< ", DEF: " << allies[i].Settings.DEF << ")" << std::endl;
		}
		std::cout << std::endl;
	}
};
