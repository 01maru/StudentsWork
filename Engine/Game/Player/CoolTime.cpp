#include "CoolTime.h"
#include "Easing.h"

using namespace Easing;

void CoolTime::Initialize()
{
	coolTimer_.SetIsActive(false);
	coolTimer_.SetIsIncrement(true);
	coolTimer_.SetFrameCountIsMax();
	isActive_ = true;
}

void CoolTime::SpriteUpdate()
{
	float start = 1.0f;
	float end = 0.0f;
	float rate = lerp(start, end, coolTimer_.GetCountPerMaxCount());

	Vector2D size = sprite_.GetSize();

	float gaugeLen = size.y * rate;

	size.y = gaugeLen;
	gauge_.SetSize(size);

	sprite_.Update();
	text_.Update();
	gauge_.Update();
}

void CoolTime::Update()
{
	coolTimer_.Update();

	if (coolTimer_.GetIsActive() == false) {
		isActive_ = true;
	}

	SpriteUpdate();
}

void CoolTime::Draw()
{
	sprite_.Draw();
	text_.Draw();
	
	//	クールタイム中のみ表示
	if (coolTimer_.GetIsActive()) {
		gauge_.Draw();
	}
}

void CoolTime::StartCount()
{
	coolTimer_.StartCount();
	isActive_ = false;
}

bool CoolTime::GetIsActive()
{
	return isActive_;
}

void CoolTime::SetSprite(const MNE::Sprite& sprite)
{
	sprite_ = sprite;

	gauge_.Initialize();
	gauge_.SetPosition(sprite_.GetPosition());
	gauge_.SetAnchorPoint(sprite_.GetAnchorPoint());
	float alpha = 0.3f;
	float gray = 0.3f;
	Vector4D color(gray, gray, gray, alpha);
	gauge_.SetColor(color);
}

void CoolTime::SetTextSprite(const MNE::Sprite& sprite)
{
	text_ = sprite;
}

void CoolTime::SetMaxTime(int32_t time)
{
	coolTimer_.SetMaxFrameCount(time);
}
