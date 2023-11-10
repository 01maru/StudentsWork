#include "CharacterHP.h"
#include "Easing.h"

void CharacterHP::Initialize()
{
	hp_ = maxHP_;
	isAlive_ = true;
}

void CharacterHP::Update()
{
	IsAliveUpdate();

	HPBarUpdate();
}

void CharacterHP::Draw()
{
	bar_.Draw();
}

void CharacterHP::IsAliveUpdate()
{
	if (hp_ <= 0) {
		isAlive_ = false;
	}
}

void CharacterHP::HPBarUpdate()
{
	Vector2D size = bar_.GetSize();

	size.x = Easing::lerp(0.0f, len_, hp_ / static_cast<float>(maxHP_));

	size.x = MyMath::mMax(size.x, 0.0f);
	bar_.SetSize(size);

	bar_.Update();
}

int32_t CharacterHP::GetHP()
{
	return hp_;
}

int32_t CharacterHP::GetMaxHP()
{
	return maxHP_;
}

bool CharacterHP::GetIsAlive()
{
	return isAlive_;
}

void CharacterHP::SetMaxHP(int32_t maxHP)
{
	maxHP_ = maxHP;
}

void CharacterHP::SetSprite(Sprite& sprite)
{
	bar_ = sprite;
	len_ = bar_.GetSize().x;
}
