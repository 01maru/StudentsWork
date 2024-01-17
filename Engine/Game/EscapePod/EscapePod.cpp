#include "EscapePod.h"
#include "PodBefCollGroundState.h"
#include "TextureManager.h"
#include "Easing.h"

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
	GetAnimation()->SetAnimationIdx(0);

	ui_.Initialize(TextureManager::GetInstance()->GetTextureGraph("space.png"));
	ui_.SetPosition({ 750,400 });

	fadeCounter_.Initialize(30, true);
}

void EscapePod::LoadResources()
{
	TextureManager::GetInstance()->LoadTextureGraph("space.png");
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
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void EscapePod::DrawUI()
{
	if (drawUI_ == FALSE) return;

	ui_.Draw();
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
