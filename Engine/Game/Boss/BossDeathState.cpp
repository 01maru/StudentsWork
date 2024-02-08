#include "BossDeathState.h"
#include "Boss.h"
#include "Easing.h"
#include "LightManager.h"

#include "ParticleManager.h"
#include "DeadParticleEmitter.h"

using namespace MNE;
using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossDeathState::Initialize()
{
	//	タイマー初期化
	timer_.Initialize(animationTime_, TRUE, TRUE);
	timer_.StartCount();
	fogTimer_.Initialize(fogTime_, TRUE);

	//	エミッターの生成
	DeadParticleEmitter emitter;
	emitter_ = ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());
	emitter_->SetPosition(sBoss_->GetPosition());

	//	アニメーション設定
	sBoss_->GetAnimation()->SetAnimeName("Dying");
	sBoss_->GetAnimation()->SetIsLoop(FALSE);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossDeathState::Update()
{
	//	タイマー更新
	timer_.Update();

	//	アニメーション更新
	sBoss_->GetAnimation()->SetAnimeTimer(static_cast<float>(timer_.GetFrameCount()));

	FogColorUpdate();

	//	次のステートへ
	if (timer_.GetIsActive() == FALSE) {

		sBoss_->StartClearState();

		if (emitter_ != nullptr) {
			emitter_->SetIsDead(TRUE);
			emitter_ = nullptr;
		}

		std::unique_ptr<BossState> next_ = std::make_unique<BossState>();
		sBoss_->SetCurrentState(next_);
	}
}

void BossDeathState::FogStartCount()
{
	if (timer_.GetFrameCount() < fogStartTime_) return;

	if (timer_.GetFrameCount() == fogStartTime_)
	{
		fogTimer_.StartCount();
	}
}

void BossDeathState::FogColorUpdate()
{
	FogStartCount();

	if (fogTimer_.GetIsActive() == FALSE) return;

	//	タイマー更新
	fogTimer_.Update();

	Vector3D color = EaseIn(startColor_, endColor_, fogTimer_.GetCountPerMaxCount(), Single);
	LightManager::GetInstance()->SetFogColor(color);

	float value = EaseIn(fogSStart_, fogEStart_, fogTimer_.GetCountPerMaxCount(), Single);
	LightManager::GetInstance()->SetFogStart(value);

	value = EaseIn(fogSEnd_, fogEEnd_, fogTimer_.GetCountPerMaxCount(), Single);
	LightManager::GetInstance()->SetFogEnd(value);

	value = EaseIn(fogSNear_, fogENear_, fogTimer_.GetCountPerMaxCount(), Single);
	LightManager::GetInstance()->SetFogNear(value);
}
