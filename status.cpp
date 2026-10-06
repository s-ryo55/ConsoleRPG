#include <iostream>
#include "Status.h"

// hero の実体はここに置く（翻訳単位は一つだけ）
PlayerSettings hero = {
	"勇者", 1000, 100, 100, 50, 10
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

