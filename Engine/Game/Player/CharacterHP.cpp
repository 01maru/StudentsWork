#include "CharacterHP.h"
#include "Easing.h"

void CharacterHP::Initialize()
{
	hp_ = maxHP_;
	isAlive_ = true;

	damageTimer_.Initialize(animationTime_, true);
}

void CharacterHP::Update()
{
	HPBarUpdate();
}

void CharacterHP::Draw()
{
	back_.Draw();

	if (damageTimer_.GetIsActive() == true)
	{
		damage_.Draw();
	}

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
	damageTimer_.Update();
	Vector2D size = damage_.GetSize();
	size.x = Easing::EaseIn(damageLen_, 0.0f, damageTimer_.GetCountPerMaxCount(), 3);
	damage_.SetSize(size);

	bar_.Update();
	damage_.Update();
}

void CharacterHP::DecHp(int32_t damage)
{
	if (isAlive_ == false) return;

	hp_ = MyMath::mMax(0, hp_ - damage);

	IsAliveUpdate();

	Vector2D size = bar_.GetSize();
	size.x = Easing::lerp(0.0f, len_, hp_ / static_cast<float>(maxHP_));
	size.x = MyMath::mMax(size.x, 0.0f);
	bar_.SetSize(size);

	if (damage > 0) {
		damageLen_ = Easing::lerp(0.0f, len_, damage / static_cast<float>(maxHP_));

		Vector2D pos = bar_.GetPosition();
		pos.x += size.x;
		damage_.SetPosition(pos);

		damageTimer_.StartCount();
	}
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

void CharacterHP::SetSprite(const Sprite& sprite)
{
	bar_ = sprite;
	len_ = bar_.GetSize().x;
	damage_.Initialize(bar_.GetTexture());
	damage_.SetSize(bar_.GetSize());
	damage_.SetPosition(bar_.GetPosition());
	damage_.SetAnchorPoint(bar_.GetAnchorPoint());
	Vector4D redColor(1.0f, 0.0f, 0.0f, 0.4f);
	damage_.SetColor(redColor);

	back_.Initialize();
	back_.SetSize(bar_.GetSize());
	back_.SetPosition(bar_.GetPosition());
	back_.SetAnchorPoint(bar_.GetAnchorPoint());
	Vector4D grayColor(0.1f, 0.1f, 0.1f, 0.5f);
	back_.SetColor(grayColor);
	back_.Update();
}

void CharacterHP::SetBarColor(const Vector3D& color)
{
	bar_.SetColor(color);
}
