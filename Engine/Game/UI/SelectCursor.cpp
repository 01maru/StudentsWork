#include "SelectCursor.h"
#include "TextureManager.h"
#include "Easing.h"
#include "XAudioManager.h"

using namespace Easing;
using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void SelectCursor::Initialize()
{
	//	カーソルスプライト
	Texture* cursorTex = TextureManager::GetInstance()->GetTextureGraph("select.png");
	cursor_.Initialize(cursorTex);
	const Vector2D centerPoint(0.5f, 0.5f);
	cursor_.SetAnchorPoint(centerPoint);

	//	アニメーションカウンター
	counter_.Initialize(ANIME_TIME, TRUE, TRUE);
	counter_.SetIsEndless(TRUE);
	counter_.SetIsActive(TRUE);
}

void SelectCursor::LoadResources()
{
	//	カーソル画像
	TextureManager::GetInstance()->AsyncLoadTextureGraph("select.png");

	//	カーソル移動音
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void SelectCursor::AnimationUpdate()
{
	counter_.Update();

	Vector2D size = EaseIn(minSize_, maxSize_, counter_.GetCountPerMaxCount(), easePawNum_);

	cursor_.SetSize(size);
}

void SelectCursor::Update()
{
	//	アクティブじゃなかったら終了
	if (isActive_ == FALSE) return;

	AnimationUpdate();
	cursor_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void SelectCursor::Draw()
{
	//	アクティブじゃなかったら描画しない
	if (isActive_ == FALSE) return;

	cursor_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SelectCursor::SetCursorPosition(const MyMath::Vector2D& pos, bool playMoveSound)
{
	//	カーソルの位置変更されたか
	Vector2D cursorPos = cursor_.GetPosition();
	if (cursorPos == pos) return;

	//	カーソル移動音再生
	if (playMoveSound == TRUE)
	{
		XAudioManager::GetInstance()->PlaySoundWave("cursorMove.wav", XAudioManager::SE);
	}

	//	位置変更
	cursor_.SetPosition(pos);
}

void SelectCursor::SetButtonSize(const MyMath::Vector2D& size)
{
	minSize_ = size;
	maxSize_ = size + gapSize_;
}

void SelectCursor::SetIsActive(bool isActive)
{
	isActive_ = isActive;
}
