#pragma once
#include <cstdint>

/**
* @file CollAttribute.h
* @brief 当たり判定の属性の値を定義するファイル
*/

namespace CollAttribute {
	//	地形
	const uint16_t COLLISION_ATTR_LANDSHAPE = 0b1 << 0;
	//	自機
	const uint16_t COLLISION_ATTR_ALLIES = 0b1 << 1;
	//	自機の攻撃
	const uint16_t COLLISION_ATTR_ALLIES_AT = 0b1 << 2;
	//	敵
	const uint16_t COLLISION_ATTR_ENEMYS = 0b1 << 3;
	//	敵の攻撃
	const uint16_t COLLISION_ATTR_ENEMY_AT = 0b1 << 4;
}

