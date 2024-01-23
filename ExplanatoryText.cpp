#include "ExplanatoryText.h"
#include "TextureManager.h"
#include "InputManager.h"

using namespace MNE;

void ExplanatoryText::Initialize()
{
	cursor_.Initialize(cursorTex_);
	cursor_.SetSize(cursorTex_->GetTextureSize() / 2.0f);
}

void ExplanatoryText::LoadResources(const std::string& uiFileName, const std::string& cursorSpriteName)
{
	cursorTex_ = TextureManager::GetInstance()->LoadTextureGraph(cursorSpriteName);

	LoadData(uiFileName);
}

void ExplanatoryText::Update(bool prevUsePad)
{
	//	PadとKeyMouseの表示切り替え
	bool usePad = InputManager::GetInstance()->GetUsePad();
	if (prevUsePad != usePad)
	{
		if (usePad == TRUE)
		{
			//	説明テキスト表示
			ResetAnimation(true);
		}
		else
		{
			//	説明テキスト非表示
			ResetAnimation(false);
			//	マウス位置を画面中央に(マウスが画面に必ず表示されるように)
			InputManager::GetInstance()->GetMouse()->SetCursorWinCenter();
		}
	}

	if (nextTag_ != "" && GetIsEndAnimation() == TRUE)
	{
		activeTag_ = nextTag_;
		ResetAnimation(true);
		nextTag_ = "";
	}

	cursor_.SetPosition(InputManager::GetInstance()->GetMouse()->GetCursor());

	cursor_.Update();
	
	UIData::Update();
}

void ExplanatoryText::Draw()
{
	if (draw_ == FALSE && GetIsEndAnimation() == TRUE) return;

	//	マウス使用中かつアニメーション終了時
	if (InputManager::GetInstance()->GetUsePad() == TRUE)
	{
		UIData::Draw();
	}
	//	パッド使用中は表示しない
	else
	{
		cursor_.Draw();
	}
}

void ExplanatoryText::SetDrawFlag(bool flag)
{
	draw_ = flag;
}

void ExplanatoryText::SetNextAnimetionTag(const std::string& tagName, bool playAnimation, bool startingAnimation)
{
	if (activeTag_ == tagName) return;

	if (playAnimation) {
		if (startingAnimation)
		{
			ResetAnimation(true);
			activeTag_ = tagName;
		}
		else
		{
			nextTag_ = tagName;
			ResetAnimation(false);
		}
	}
	else
	{
		activeTag_ = tagName;
	}
}
