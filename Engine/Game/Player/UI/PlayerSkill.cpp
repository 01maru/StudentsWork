#include "PlayerSkill.h"
#include "InputManager.h"

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PlayerSkill::Initialize()
{
	isActive_ = TRUE;
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PlayerSkill::Update()
{
	//	パッド使用時
	if (InputManager::GetInstance()->GetUsePad() == TRUE)
	{
		//	パッドの操作方法表記
		text_.SetTexture(padTex_);
	}
	else
	{
		//	キーマウの操作方法表記
		text_.SetTexture(keyTex_);
	}
	
	sprite_.Update();
	text_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void PlayerSkill::Draw()
{
	sprite_.Draw();
	text_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool PlayerSkill::GetIsActive()
{
	return isActive_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void PlayerSkill::SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text, MNE::Texture* padTex)
{
	sprite_ = sprite;
	text_ = text;
	keyTex_ = text_.GetTexture();
	padTex_ = padTex;
}
