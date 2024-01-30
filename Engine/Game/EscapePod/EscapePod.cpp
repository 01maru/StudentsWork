#include "EscapePod.h"
#include "PodBefCollGroundState.h"
#include "UIData.h"

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void EscapePod::Initialize(const Vector3D& pos)
{
	//	Object3D初期化
	Object3D::Initialize();
	
	//	位置設定(ステート設定前に位置設定する)
	mat_.trans_ = pos;
	
	//	State初期化
	//	親ポッドの設定
	EscPodState::SetPod(this);
	//	ステート初期化
	ResetAnimation();

	//	アニメーション初期化
	GetAnimation()->SetAnimeName("PrevOpen");
	GetAnimation()->SetIsLoop(FALSE);

	//	説明のスプライト初期化
	ui_.Initialize();
}

void EscapePod::LoadResources()
{
	//	説明のスプライト用リソース読み込み
	ui_.LoadResources();
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

	//	入力説明用スプライト更新
	ui_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void EscapePod::DrawUI()
{
	//	入力説明用スプライト描画
	ui_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MNE::UIData* EscapePod::GetLetterBoxPtr()
{
	return pLetterBox_;
}

bool EscapePod::GetOpenDoor()
{
	return openDoor_;
}

bool EscapePod::GetDrawPlayer()
{
	return drawPlayer_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void EscapePod::SetInputUISprite(const MNE::Sprite& uiSprite, const MNE::Sprite& buttonSprite)
{
	ui_.SetSprite(uiSprite, buttonSprite);
}

void EscapePod::StartUICounter()
{
	ui_.Start();
}

void EscapePod::SetUIIsActive(bool isActive)
{
	ui_.SetIsAcitve(isActive);
}

void EscapePod::SetLetterBox(MNE::UIData* data)
{
	pLetterBox_ = data;
}

void EscapePod::SetDrawPlayer(bool drawplayer)
{
	drawPlayer_ = drawplayer;
}

void EscapePod::SetOpenDoor(bool openDoor)
{
	openDoor_ = openDoor;
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
