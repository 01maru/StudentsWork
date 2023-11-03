#include "SelectCursor.h"
#include "TextureManager.h"
#include "Easing.h"
#include "XAudioManager.h"

using namespace Easing;

void SelectCursor::Initialize()
{
	cursor_.Initialize(TextureManager::GetInstance()->GetTextureGraph("select.png"));
	cursor_.SetPosition(Vector2D(200, 420));
	Vector2D centerPoint(0.5f, 0.5f);
	cursor_.SetAnchorPoint(centerPoint);

	counter_.Initialize(40, true, true);
	counter_.SetIsEndless(true);
	counter_.SetIsActive(true);
}

void SelectCursor::LoadResources()
{
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
}

void SelectCursor::Update()
{
	counter_.Update();

	Vector2D size = Easing::EaseIn(minSize_, maxSize_, counter_.GetCountPerMaxCount(), 2);

	cursor_.SetSize(size);

	cursor_.Update();
}

void SelectCursor::Draw()
{
	cursor_.Draw();
}

void SelectCursor::SetCursorPosition(const Vector2D& pos)
{
	if (cursor_.GetPosition() == pos) return;

	//	カーソル移動音再生
	XAudioManager::GetInstance()->PlaySoundWave("cursorMove.wav", XAudioManager::SE);

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
