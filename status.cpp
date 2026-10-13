#include <iostream>
#include "Status.h"

// hero の実体はここに置く（翻訳単位は一つだけ）
PlayerSettings hero = {
	"勇者", 1000, 100, 100, 50, 10
};

// ソルジャー（高HP、高DEF、低SPD）
PlayerSettings soldier = {
	"ソルジャー", 1200, 80, 80, 70, 8
};

// ヒーラー（低HP、高MP、低ATK、高DEF）
PlayerSettings healer = {
	"ヒーラー", 700, 200, 60, 60, 12
};

// レンジャー（低DEF、高SPD、中程度ATK）
PlayerSettings ranger = {
	"レンジャー", 800, 120, 110, 40, 15
};

// enemy ではなく slime として定義（Status.h の extern と一致させる）
EnemySettings slime = {
	"スライム", 100, 50, 20, 10, 5
};

// dragon の実体もここに置く
BossSettings dragon = {
	"ドラゴン", 5000, 200, 300, 150, 20
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

void Boss::Show() const
{
	std::cout << "Name: " << Settings.Name
		<< ", HP: " << Settings.HP
		<< ", MP: " << Settings.MP
		<< ", ATK: " << Settings.ATK
		<< ", DEF: " << Settings.DEF
		<< ", SPD: " << Settings.SPD
		<< std::endl;
}
