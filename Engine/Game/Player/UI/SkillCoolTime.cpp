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

	effect_.LoadResources();
	effect_.Initialize();

	shakeTimer_.Initialize(shakeTime_, FALSE);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void SkillCoolTime::GaugeUpdate()
{
	//	クールタイム中じゃなかったら
	if (coolTimer_.GetIsActive() == FALSE) {

		if (isActive_ == FALSE) {
			effect_.Start();
		}

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

void SkillCoolTime::ShakeUpdate()
{
	if (shakeTimer_.GetIsActive() == FALSE) return;
	shakeTimer_.Update();

	float shakeMaxX = lerp(0.0f, shakeMax_, shakeTimer_.GetCountPerMaxCount());
	float rotMax = lerp(0.0f, rotMax_, shakeTimer_.GetCountPerMaxCount());
	float shakePos_ = GetRand(-shakeMaxX, shakeMaxX);
	float rotVal_ = GetRand(-rotMax, rotMax);
	Vector2D pos = centerPos_;
	pos.x += shakePos_;

	gauge_.SetRotation(rotVal_);
	gauge_.SetPosition(pos);
	sprite_.SetRotation(rotVal_);
	sprite_.SetPosition(pos);
}

void SkillCoolTime::Update()
{
	//	クールタイム更新
	coolTimer_.Update();

	//	ゲージの動き更新
	GaugeUpdate();

	ShakeUpdate();

	//	スプライトの更新
	PlayerSkill::Update();
	gauge_.Update();

	effect_.Update();
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

	effect_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SkillCoolTime::StartShakeAnime()
{
	if (shakeTimer_.GetIsActive() == TRUE) return;

	shakeTimer_.StartCount();
}

void SkillCoolTime::StartCount()
{
	coolTimer_.StartCount();
	isActive_ = FALSE;
}

void SkillCoolTime::SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text, MNE::Texture* padTex)
{
	PlayerSkill::SetSprite(sprite, text, padTex);

	centerPos_ = sprite.GetPosition();

	//	スキルのスプライトに合わせてゲージを初期化
	gauge_.Initialize();
	gauge_.SetPosition(centerPos_);
	gauge_.SetSize(sprite.GetSize());
	gauge_.SetAnchorPoint(sprite.GetAnchorPoint());
	float alpha = 0.8f;
	float gray = 0.1f;
	MyMath::Vector4D color(gray, gray, gray, alpha);
	gauge_.SetColor(color);

	Vector2D startPos = centerPos_;
	startPos.y += gauge_.GetSize().y;
	
	effect_.SetStartPos(startPos);
}

void SkillCoolTime::SetMaxTime(int32_t time)
{
	coolTimer_.SetMaxFrameCount(time);
}
