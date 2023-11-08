#pragma once
#include <cstdint>

class CharacterHP
{
public:
	void Initialize();
	void Update();

protected:
	int32_t hp_;
	int32_t maxHP_ = 100;
	bool isAlive_ = false;

public:
	int32_t GetHP();
	bool GetIsAlive();
	void SetMaxHP(int32_t maxHP);
};

