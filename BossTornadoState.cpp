#include "BossTornadoState.h"
#include "Boss.h"
#include "Player.h"
#include "BossIdleState.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace MyMath;
using namespace MNE;
using namespace MNE::CollAttribute;

void BossTornadoState::Initialize()
{
	stateTable_ = { &BossTornadoState::WaitStateUpdate,
		&BossTornadoState::AttackStateUpdate,
		&BossTornadoState::EndStateUpdate ,
		&BossTornadoState::StanStateUpdate };

	WaitStateInitialize();

	emptyObj_.SetPosition(sBoss_->GetPosition());
	emptyObj_.Initialize();
	emptyObj_.SetSuctionValue(sBoss_->GetSuctionValue());
}

void BossTornadoState::WaitStateInitialize()
{
	timer_.Initialize(sBoss_->GetTornadoWaitTime(), true);
	timer_.StartCount();

	nowState_ = WaitState;
}

void BossTornadoState::AttackStateInitialize()
{
	timer_.Initialize(sBoss_->GetTornadoAtTime(), true);
	timer_.StartCount();

	nowState_ = AttackState;
}

void BossTornadoState::EndStateInitialize()
{
	timer_.Initialize(sBoss_->GetTornadoEndTime(), true);
	timer_.StartCount();

	nowState_ = EndState;
}

void BossTornadoState::StanStateInitialize()
{
	timer_.Initialize(sBoss_->GetTornadoStanTime(), true);
	timer_.StartCount();

	nowState_ = StanState;
}

void BossTornadoState::WaitStateUpdate()
{
	if (timer_.GetIsActive() == FALSE) {
		//	攻撃ステートへ
		AttackStateInitialize();
	}
}

void BossTornadoState::AttackStateUpdate()
{
	//	拡大
	float rad = emptyObj_.GetRadius() + 2.0f;
	rad = mMin(rad, sBoss_->GetTornadoMaxR());
	emptyObj_.SetRadius(rad);

	//	回転
	float norm = mMin(1.0f, rad / sBoss_->GetTornadoMaxR());
	Vector3D rot = sBoss_->GetRotation();
	rot.y += sBoss_->GetTornadoRotSpd() * norm;
	sBoss_->SetRotation(rot);

	//	吸い込みチェック
	emptyObj_.Update();

	if (timer_.GetIsActive() == FALSE) {
		//	終了ステートへ
		EndStateInitialize();
	}
}

void BossTornadoState::EndStateUpdate()
{
	//	渦消える

	//	回転徐々にゆっくり

	if (timer_.GetIsActive() == FALSE) {
		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}

void BossTornadoState::StanStateUpdate()
{
	if (timer_.GetIsActive() == FALSE) {
		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}

void BossTornadoState::Update()
{
	timer_.Update();

	//	現在のステートの更新
	(this->*stateTable_[nowState_])();
}
