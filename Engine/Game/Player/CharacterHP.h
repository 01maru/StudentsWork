#pragma once
#include <cstdint>
#include "Sprite.h"
#include "FrameCounter.h"

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
	FrameCounter damageTimer_;
	float damageLen_ = 0.0f;

	Sprite back_;
	Sprite damage_;
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
	void SetBarColor(const Vector3D& color);
};

