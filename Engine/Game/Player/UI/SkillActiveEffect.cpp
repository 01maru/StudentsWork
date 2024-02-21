#include "SkillActiveEffect.h"
#include "TextureManager.h"
#include "Easing.h"

using namespace Easing;
using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void SkillActiveEffect::LoadResources()
{
	Texture* tex = TextureManager::GetInstance()->LoadTextureGraph("particle2.png");
	texSize_ = tex->GetTextureSize();
}

void SkillActiveEffect::Initialize()
{
	Texture* tex = TextureManager::GetInstance()->GetTextureGraph("particle2.png");

	sprite_.Initialize(tex);
}

void SkillActiveEffect::InitEndAnimation()
{
	timer_.Initialize(endTime_, FALSE);
	timer_.StartCount();

	Vector2D centerPoint(0.5f, 0.5f);
	sprite_.SetAnchorPoint(centerPoint);
	sprite_.SetPosition(endPos_);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void SkillActiveEffect::StartAnimeUpdate()
{
	if (timer_.GetIsIncrement() == FALSE) return;

	Vector2D size = sprite_.GetSize();
	size.y = EaseIn(0.0f, maxSizeY_, timer_.GetCountPerMaxCount(), Double);
	sprite_.SetSize(size);

	if (timer_.GetIsActive() == FALSE)
	{
		InitEndAnimation();
	}
}

void SkillActiveEffect::EndAnimeUpdate()
{
	if (timer_.GetIsIncrement() == TRUE) return;

	Vector2D size = sprite_.GetSize();
	size.x = EaseIn(0.0f, texSize_.x, timer_.GetCountPerMaxCount(), Double);
	sprite_.SetSize(size);
}

void SkillActiveEffect::Update()
{
	if (timer_.GetIsActive() == FALSE) return;
	
	timer_.Update();

	EndAnimeUpdate();
	StartAnimeUpdate();

	sprite_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void SkillActiveEffect::Draw()
{
	if (timer_.GetIsActive() == FALSE) return;

	sprite_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SkillActiveEffect::Start()
{
	timer_.Initialize(startTime_, TRUE);
	timer_.StartCount();

	Vector2D midBot(0.5f, 1.0f);
	sprite_.SetAnchorPoint(midBot);
	sprite_.SetPosition(startPos_);
	sprite_.SetSize(texSize_);
}

void SkillActiveEffect::SetStartPos(const MyMath::Vector2D& pos)
{
	startPos_ = pos;
	endPos_ = startPos_;
	float halfSizeY_ = maxSizeY_ / 2.0f;
	endPos_.y -= halfSizeY_;
}
