#include "CoolTime.h"
#include "Easing.h"

using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void SkillCoolTime::Initialize()
{
	coolTimer_.SetIsActive(false);
	coolTimer_.SetIsIncrement(true);
	coolTimer_.SetFrameCountIsMax();
	
	PlayerSkill::Initialize();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void SkillCoolTime::GaugeUpdate()
{
	//	クールタイム中じゃなかったら
	if (coolTimer_.GetIsActive() == FALSE) {
		isActive_ = true;
		return;
	}

	float start = 1.0f;
	float end = 0.0f;
	float rate = lerp(start, end, coolTimer_.GetCountPerMaxCount());

	Vector2D size = sprite_.GetSize();

	float gaugeLen = size.y * rate;

	size.y = gaugeLen;
	gauge_.SetSize(size);
}

void SkillCoolTime::Update()
{
	coolTimer_.Update();

	GaugeUpdate();

	PlayerSkill::Update();
	gauge_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void SkillCoolTime::Draw()
{
	PlayerSkill::Draw();
	
	//	クールタイム中のみ表示
	if (coolTimer_.GetIsActive() == TRUE)
	{
		gauge_.Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SkillCoolTime::StartCount()
{
	coolTimer_.StartCount();
	isActive_ = false;
}

void SkillCoolTime::SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text)
{
	PlayerSkill::SetSprite(sprite, text);

	gauge_.Initialize();
	gauge_.SetPosition(sprite.GetPosition());
	gauge_.SetAnchorPoint(sprite.GetAnchorPoint());
	float alpha = 0.3f;
	float gray = 0.3f;
	Vector4D color(gray, gray, gray, alpha);
	gauge_.SetColor(color);
}

void SkillCoolTime::SetMaxTime(int32_t time)
{
	coolTimer_.SetMaxFrameCount(time);
}
