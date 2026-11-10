#include "Status.h"

// キャラクターの初期データ（Model）
// ※ 名前・数値は元のコードに合わせて必ず確認してください
//   並びは { 名前, HP, MP, ATK, DEF, SPD, 種類 }

PlayerSettings hero    = { "勇者",       1000, 100, 100, 50, 10 };
PlayerSettings soldier = { "ソルジャー", 1200,  80,  80, 70,  8, CharType::Soldier };
PlayerSettings healer  = { "ヒーラー",    700, 200,  60, 60, 12, CharType::Healer };
PlayerSettings ranger  = { "レンジャー",  800, 120, 110, 40, 15, CharType::Ranger };

EnemySettings slime  = { "スライム", 100,  50,  20,  10,  5 };
BossSettings  dragon = { "ドラゴン", 5000, 200, 300, 150, 20 };
