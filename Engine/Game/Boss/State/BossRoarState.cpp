#include "BossRoarState.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "LightManager.h"
#include "Easing.h"

using namespace MNE;
using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossRoarState::Initialize()
{
	//	タイマー初期化&スタート
	timer_.Initialize(roarTime_, TRUE);
	timer_.StartCount();
	fogTimer_.Initialize(fogTime_, TRUE);

	//	アニメーション設定
	sBoss_->GetAnimation()->SetAnimeName("Roaring");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossRoarState::Update()
{
	//	タイマー更新
	timer_.Update();

	//	アニメーション更新
	sBoss_->GetAnimation()->SetAnimeTimer(static_cast<float>(timer_.GetFrameCount()));

	FogColorUpdate();

	//	次のステートへ
	if (timer_.GetIsActive() == FALSE) {
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}

void BossRoarState::FogStartCount()
{
	if (timer_.GetFrameCount() < fogStartTime_) return;

	if (timer_.GetFrameCount() == fogStartTime_)
	{
		fogTimer_.StartCount();
	}
}

void BossRoarState::FogColorUpdate()
{
	FogStartCount();

	if (fogTimer_.GetIsActive() == FALSE) return;

	//	タイマー更新
	fogTimer_.Update();

	Vector3D color = EaseOut(startColor_, endColor_, fogTimer_.GetCountPerMaxCount(), Triple);
	LightManager::GetInstance()->SetFogColor(color);

	float value = EaseOut(fogSStart_, fogEStart_, fogTimer_.GetCountPerMaxCount(), Triple);
	LightManager::GetInstance()->SetFogStart(value);

	value = EaseOut(fogSEnd_, fogEEnd_, fogTimer_.GetCountPerMaxCount(), Triple);
	LightManager::GetInstance()->SetFogEnd(value);

	value = EaseOut(fogSNear_, fogENear_, fogTimer_.GetCountPerMaxCount(), Triple);
	LightManager::GetInstance()->SetFogNear(value);
}
