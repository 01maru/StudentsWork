#pragma once
#include <cstdint>

namespace CollAttribute {
	const uint16_t COLLISION_ATTR_LANDSHAPE = 0b1 << 0;
	const uint16_t COLLISION_ATTR_ALLIES = 0b1 << 1;
	const uint16_t COLLISION_ATTR_ENEMYS = 0b1 << 2;
	const uint16_t COLLISION_ATTR_ENEMY_AT = 0b1 << 2;
}

