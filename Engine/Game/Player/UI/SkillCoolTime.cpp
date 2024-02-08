#include "SkillCoolTime.h"
#include "Easing.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void SkillCoolTime::Initialize()
{
	//	クールタイムタイマー初期化
	coolTimer_.SetIsActive(FALSE);
	coolTimer_.SetIsIncrement(TRUE);
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
		//	使用可能状態に(以下の処理しない)
		isActive_ = TRUE;
		return;
	}

	//	以下ゲージの動き処理
	float rate = lerp(1.0f, 0.0f, coolTimer_.GetCountPerMaxCount());

	Vector2D size = sprite_.GetSize();

	float gaugeLen = size.y * rate;

	size.y = gaugeLen;
	gauge_.SetSize(size);
}

void SkillCoolTime::Update()
{
	//	クールタイム更新
	coolTimer_.Update();

	//	ゲージの動き更新
	GaugeUpdate();

	//	スプライトの更新
	PlayerSkill::Update();
	gauge_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void SkillCoolTime::Draw()
{
	//	スキルや操作方法の描画
	PlayerSkill::Draw();
	
	//	クールタイム中のみ表示
	if (coolTimer_.GetIsActive() == TRUE)
	{
		//	ゲージの描画
		gauge_.Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SkillCoolTime::StartCount()
{
	coolTimer_.StartCount();
	isActive_ = FALSE;
}

void SkillCoolTime::SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text)
{
	PlayerSkill::SetSprite(sprite, text);

	//	スキルのスプライトに合わせてゲージを初期化
	gauge_.Initialize();
	gauge_.SetPosition(sprite.GetPosition());
	gauge_.SetAnchorPoint(sprite.GetAnchorPoint());
	float alpha = 0.3f;
	float gray = 0.3f;
	MyMath::Vector4D color(gray, gray, gray, alpha);
	gauge_.SetColor(color);
}

void SkillCoolTime::SetMaxTime(int32_t time)
{
	coolTimer_.SetMaxFrameCount(time);
}
