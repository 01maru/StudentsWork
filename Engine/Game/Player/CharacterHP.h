#pragma once
#include <cstdint>
#include "Sprite.h"

class CharacterHP
{
public:
	void Initialize();
	void Update();
	void Draw();

protected:
	int32_t hp_;
	int32_t maxHP_ = 100;
	bool isAlive_ = false;

	Sprite bar_;
	float len_;

	void IsAliveUpdate();
	void HPBarUpdate();
public:
	void DecHp(int32_t damage);
	int32_t GetHP();
	int32_t GetMaxHP();
	bool GetIsAlive();
	void SetMaxHP(int32_t maxHP);
	void SetSprite(Sprite& sprite);
};

