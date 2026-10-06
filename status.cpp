#include <iostream>
#include "Status.h"

// hero の実体はここに置く（翻訳単位は一つだけ）
PlayerSettings hero = {
	"勇者", 1000, 100, 100, 50, 10
};

// enemy ではなく slime として定義（Status.h の extern と一致させる）
EnemySettings slime = {
	"スライム", 100, 50, 20, 10, 5
};

void Player::Show() const
{
	std::cout << "Name: " << Settings.Name
		<< ", HP: " << Settings.HP
		<< ", MP: " << Settings.MP
		<< ", ATK: " << Settings.ATK
		<< ", DEF: " << Settings.DEF
		<< ", SPD: " << Settings.SPD
		<< std::endl;
}

// Enemy::Show の実装を追加
void Enemy::Show() const
{
	std::cout << "Name: " << Settings.Name
		<< ", HP: " << Settings.HP
		<< ", MP: " << Settings.MP
		<< ", ATK: " << Settings.ATK
		<< ", DEF: " << Settings.DEF
		<< ", SPD: " << Settings.SPD
		<< std::endl;
}

