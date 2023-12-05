#include "BossMeleeState.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "Easing.h"
#include "Player.h"

using namespace Easing;

void BossMeleeState::Initialize()
{
	timer_.Initialize(60, true);
	timer_.StartCount();

	attackTime = 60;

	startAngle_ = sBoss_->GetRot();
	startPos_ = sBoss_->GetPosition();

	frontVec_ = sBoss_->GetPosition();
	frontVec_ -= sBoss_->GetPlayer()->GetPosition();
	frontVec_.y = 0.0f;
	frontVec_.Normalize();
}

void BossMeleeState::Update()
{
	timer_.Update();

	if (sBoss_->GetBodyAttack() == false) {
		float a = -MyMath::ConvertToRad(30.0f);
		Vector3D rot = sBoss_->GetRot();
		rot.x = startAngle_.x - EaseOutBack(0.0f, a, timer_.GetCountPerMaxCount(), 5);
		sBoss_->SetRot(rot);
		float len = EaseOutBack(0.0f, 6 * a / MyMath::PIx2, timer_.GetCountPerMaxCount(), 5);
		Vector3D pos = sBoss_->GetPosition();
		pos = startPos_ - frontVec_ * len;
		sBoss_->SetPosition(pos);

		if (timer_.GetIsActive() == false) {
			timer_.StartCount();
			sBoss_->SetBodyAttack(true);

			rot = sBoss_->GetRot();
			rot.y = atan2(frontVec_.x, frontVec_.z);
			sBoss_->SetRot(rot);

			startAngle_ = sBoss_->GetRot();
			startPos_ = sBoss_->GetPosition();
		}
	}
	else {
		float a = -MyMath::ConvertToRad(390.0f);
		Vector3D rot = sBoss_->GetRot();
		rot.x = startAngle_.x + Easing::EaseIn(0.0f, a, timer_.GetCountPerMaxCount(), 3);
		sBoss_->SetRot(rot);

		float len = Easing::EaseIn(0.0f, 20, timer_.GetCountPerMaxCount(), 3);
		Vector3D pos = sBoss_->GetPosition();
		pos = startPos_ - frontVec_ * len;
		sBoss_->SetPosition(pos);

		if (timer_.GetIsActive() == false) {
			//	終了
			std::unique_ptr<EnemyState> next_ = std::make_unique<BossIdleState>();
			sBoss_->SetCurrentState(next_);
			sBoss_->SetBodyAttack(false);
		}
	}
}
