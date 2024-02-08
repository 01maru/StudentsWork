#include "PodInputUI.h"
#include "Easing.h"
#include "TextureManager.h"
#include "InputManager.h"

using namespace MNE;
using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodInputUI::Initialize()
{
	counter_.Initialize(fadeMaxTime_, TRUE);
}

void PodInputUI::LoadResources()
{
	keyTex_ = TextureManager::GetInstance()->LoadTextureGraph("EKey.png");
	padTex_ = TextureManager::GetInstance()->LoadTextureGraph("XButton.png");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodInputUI::ChangeSpriteTex()
{
	//	パッド使用中
	if (InputManager::GetInstance()->GetUsePad() == TRUE)
	{
		button_.SetTexture(padTex_);
	}

	//	キーマウ使用中
	else
	{
		button_.SetTexture(keyTex_);
	}
}

void PodInputUI::Update()
{
	//	実行中じゃなかったら処理しない
	if (isActive_ == FALSE) return;

	counter_.Update();

	float fade = EaseIn(GetNormalizedMinVal(), GetNormalizedMaxVal(), counter_.GetCountPerMaxCount(), Easing::Double);
	ui_.SetAlphaColor(fade);
	button_.SetAlphaColor(fade);

	ui_.Update();
	button_.Update();

	ChangeSpriteTex();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void PodInputUI::Draw()
{
	//	実行中じゃなかったら描画しない
	if (isActive_ == FALSE) return;

	ui_.Draw();
	button_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void PodInputUI::Start()
{
	//	実行中じゃなかったら処理しない
	if (isActive_ == TRUE) return;

	isActive_ = TRUE;
	counter_.StartCount();
}

void PodInputUI::SetSprite(const MNE::Sprite& uiSprite, const MNE::Sprite& buttonSprite)
{
	ui_ = uiSprite;
	button_ = buttonSprite;
}

void PodInputUI::SetIsAcitve(bool isActive)
{
	isActive_ = isActive;
}
