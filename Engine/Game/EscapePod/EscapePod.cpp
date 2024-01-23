#include "EscapePod.h"
#include "PodBefCollGroundState.h"
#include "TextureManager.h"
#include "Easing.h"
#include "InputManager.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void EscapePod::Initialize(MNE::IModel* model, const Vector3D& pos)
{
	//	Object3D初期化
	Object3D::Initialize();
	//	モデル設定
	Object3D::SetModel(model);

	mat_.trans_ = pos;
	
	//	State初期化
	//	親ポッドの設定
	EscPodState::SetPod(this);
	ResetAnimation();
	GetAnimation()->SetAnimeName("PrevOpen");
	GetAnimation()->SetIsLoop(false);

	ui_.Initialize(TextureManager::GetInstance()->GetTextureGraph("exitPod.png"));
	ui_.SetPosition({ 750,400 });
	ui_.SetAnchorPoint({ 0.0f,0.5f });
	ui_.SetSize({ 155, 24 });
	button_.Initialize(padTex_);
	button_.SetPosition({ 730,400 });
	button_.SetAnchorPoint({ 0.5f,0.5f });
	button_.SetSize(padTex_->GetTextureSize() / 4.0f);

	fadeCounter_.Initialize(30, true);
}

void EscapePod::LoadResources()
{
	TextureManager::GetInstance()->LoadTextureGraph("exitPod.png");
	keyTex_ = TextureManager::GetInstance()->LoadTextureGraph("EKey.png");
	padTex_ = TextureManager::GetInstance()->LoadTextureGraph("XButton.png");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void EscapePod::Update()
{
	//	nullチェック
	if (currentState_ != nullptr)
	{
		currentState_->Update();
	}

	if (drawUI_ == TRUE)
	{
		fadeCounter_.Update();

		float fade = EaseIn(0.0f, 1.0f, fadeCounter_.GetCountPerMaxCount(), 2);
		ui_.SetAlphaColor(fade);
		ui_.Update();
		button_.SetAlphaColor(fade);
		button_.Update();

		if (InputManager::GetInstance()->GetUsePad() == TRUE)
		{
			button_.SetTexture(padTex_);
		}
		else
		{
			button_.SetTexture(keyTex_);
		}
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void EscapePod::DrawUI()
{
	if (drawUI_ == FALSE) return;

	ui_.Draw();
	button_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void EscapePod::StartUICounter()
{
	if (drawUI_ == FALSE)
	{
		drawUI_ = true;
		fadeCounter_.StartCount();
	}
}

void EscapePod::ResetAnimation()
{
	//	初期ステート
	std::unique_ptr<EscPodState> next = std::make_unique<PodBefCollGroundState>();
	SetNextState(next);
}

void EscapePod::SetNextState(std::unique_ptr<EscPodState>& next)
{
	currentState_ = std::move(next);

	//	nullチェック
	if (currentState_ != nullptr)
	{
		currentState_->Initialize();
	}
}
