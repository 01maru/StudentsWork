#include "ExplanatoryText.h"
#include "TextureManager.h"
#include "InputManager.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void ExplanatoryText::Initialize()
{
	cursor_.Initialize(cursorTex_);
	//	テクスチャの半分のサイズ
	Vector2D halfSize = cursorTex_->GetTextureSize() / 2.0f;
	cursor_.SetSize(halfSize);
}

void ExplanatoryText::LoadResources(const std::string& uiFileName, const std::string& cursorSpriteName)
{
	cursorTex_ = TextureManager::GetInstance()->LoadTextureGraph(cursorSpriteName);

	LoadData(uiFileName);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void ExplanatoryText::SetNextTagUpdate()
{
	//	次のタグが設定されていてUIが非表示状態だったら
	if (nextTag_ != "" && GetIsEndAnimation() == TRUE)
	{
		activeTag_ = nextTag_;
		ResetAnimation(TRUE);
		nextTag_ = "";
	}
}

void ExplanatoryText::Update(bool prevUsePad)
{
	//	PadとKeyMouseの表示切り替え
	bool usePad = InputManager::GetInstance()->GetUsePad();
	if (prevUsePad != usePad)
	{
		//	パッド使用時
		if (usePad == TRUE)
		{
			//	説明テキスト表示
			ResetAnimation(TRUE);
		}

		//	マウス使用時
		else
		{
			//	説明テキスト非表示
			ResetAnimation(FALSE);
			//	マウス位置を画面中央に(マウスが画面に必ず表示されるように)
			InputManager::GetInstance()->GetMouse()->SetCursorWinCenter();
		}
	}

	//	カーソルの位置更新
	cursor_.SetPosition(InputManager::GetInstance()->GetMouse()->GetCursor());

	SetNextTagUpdate();
	
	UIData::Update();
}

void ExplanatoryText::MatUpdate()
{
	cursor_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void ExplanatoryText::Draw()
{
	//	マウス使用中かつアニメーション終了時
	if (isActive_ == FALSE && GetIsEndAnimation() == TRUE) return;

	//	パッド使用時
	if (InputManager::GetInstance()->GetUsePad() == TRUE)
	{
		UIData::Draw();
	}

	//	マウス使用時(マウスカーソル表示)
	else
	{
		cursor_.Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void ExplanatoryText::SetIsActive(bool isActive)
{
	isActive_ = isActive;
}

void ExplanatoryText::SetNextAnimationTag(const std::string& tagName, bool playAnimation, bool startingAnimation)
{
	//	実行中のタグだったら以下処理しない
	if (activeTag_ == tagName) return;

	//	アニメーションするかどうか
	if (playAnimation) {

		//	何も表示していないとき
		if (startingAnimation)
		{
			//	表示アニメーション
			ResetAnimation(TRUE);
			activeTag_ = tagName;
		}

		//	既に表示しているなら
		else
		{
			//	表示しているものを非表示->表示になるように
			nextTag_ = tagName;
			ResetAnimation(FALSE);
		}
	}
	else
	{
		activeTag_ = tagName;
	}
}
