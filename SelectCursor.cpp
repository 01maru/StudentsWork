#include "SelectCursor.h"
#include "TextureManager.h"
#include "Easing.h"
#include "XAudioManager.h"

using namespace Easing;

void SelectCursor::Initialize()
{
	//	カーソルスプライト
	Texture* cursorTex = TextureManager::GetInstance()->GetTextureGraph("select.png");
	cursor_.Initialize(cursorTex);
	const Vector2D centerPoint(0.5f, 0.5f);
	cursor_.SetAnchorPoint(centerPoint);

	//	アニメーションカウンター
	counter_.Initialize(40, true, true);
	counter_.SetIsEndless(true);
	counter_.SetIsActive(true);
}

void SelectCursor::LoadResources()
{
	//	カーソル画像
	TextureManager::GetInstance()->AsyncLoadTextureGraph("select.png");

	//	カーソル移動音
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
}

void SelectCursor::AnimationUpdate()
{
	counter_.Update();

	Vector2D size = EaseIn(minSize_, maxSize_, counter_.GetCountPerMaxCount(), easePawNum_);

	cursor_.SetSize(size);
}

void SelectCursor::Update()
{
	//	アクティブじゃなかったら終了
	if (isActive_ == false) return;

	AnimationUpdate();
	cursor_.Update();
}

void SelectCursor::Draw()
{
	//	アクティブじゃなかったら描画しない
	if (isActive_ == false) return;

	cursor_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SelectCursor::SetCursorPosition(const Vector2D& pos, bool playMoveSound)
{
	//	カーソルの位置変更されたか
	if (cursor_.GetPosition() == pos) return;

	//	カーソル移動音再生
	if (playMoveSound == true) {
		XAudioManager::GetInstance()->PlaySoundWave("cursorMove.wav", XAudioManager::SE);
	}

	//	位置変更
	cursor_.SetPosition(pos);
}

void SelectCursor::SetMinSize(const Vector2D& size)
{
	minSize_ = size;
}

void SelectCursor::SetMaxSize(const Vector2D& size)
{
	maxSize_ = size;
}

//void SelectCursor::SetSize(const Vector2D& size)
//{
//	selectObjSize_ = size;
//
//	minSize_ = selectObjSize_ * 1.1f;
//	maxSize_ = selectObjSize_ * 1.3f;
//}

void SelectCursor::SetIsActive(bool isActive)
{
	isActive_ = isActive;
}
