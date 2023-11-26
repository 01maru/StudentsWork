#include "CharacterHP.h"
#include "Easing.h"

void CharacterHP::Initialize()
{
	hp_ = maxHP_;
	isAlive_ = true;

	damageTimer_.Initialize(30, true);
}

void CharacterHP::Update()
{
	IsAliveUpdate();

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
	hp_ = MyMath::mMax(0, hp_ - damage);

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

void CharacterHP::SetSprite(Sprite& sprite)
{
	bar_ = sprite;
	len_ = bar_.GetSize().x;
	damage_.Initialize(bar_.GetTexture());
	damage_.SetSize(bar_.GetSize());
	damage_.SetPosition(bar_.GetPosition());
	damage_.SetAnchorPoint(bar_.GetAnchorPoint());
	damage_.SetColor(Vector4D(1.0f, 0.0f, 0.0f, 0.4f));

	back_.Initialize();
	back_.SetSize(bar_.GetSize());
	Vector2D pos = bar_.GetPosition();
	pos.x += bar_.GetSize().x / 2.0f;
	back_.SetAnchorPoint({ 0.5f,0.5f });
	back_.SetPosition(pos);
	back_.SetColor(Vector4D(0.1f, 0.1f, 0.1f, 0.5f));
	back_.Update();
}

void CharacterHP::SetBarColor(const Vector3D& color)
{
	bar_.SetColor(color);
}
