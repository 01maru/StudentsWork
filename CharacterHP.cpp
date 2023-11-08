#include "CharacterHP.h"

void CharacterHP::Initialize()
{
	hp_ = maxHP_;
	isAlive_ = true;
}

void CharacterHP::Update()
{
	if (hp_ <= 0) isAlive_ = false;
}

int32_t CharacterHP::GetHP()
{
	return hp_;
}

bool CharacterHP::GetIsAlive()
{
	return isAlive_;
}

void CharacterHP::SetMaxHP(int32_t maxHP)
{
	maxHP_ = maxHP;
}
