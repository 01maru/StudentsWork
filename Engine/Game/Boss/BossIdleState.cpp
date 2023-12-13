#include "BossIdleState.h"
#include "Boss.h"
#include "BossDeathState.h"

#include "BossMeleeState.h"
#include "BossBulletState.h"
#include "BossWayBullets.h"

#include "InputManager.h"
void BossIdleState::Initialize()
{
	timer_.Initialize(idleTime_, true);
	timer_.StartCount();
}

void BossIdleState::Update()
{
	timer_.Update();
	if (sBoss_->GetIsAlive() == false) {
		std::unique_ptr<EnemyState> next_ = std::make_unique<BossDeathState>();
		sBoss_->SetCurrentState(next_);
	}
	else {
		float dis = sBoss_->RotationUpdate();
		const float MIN_DIS = 10.0f;

		if (dis > MIN_DIS) {
			Vector3D pos = sBoss_->GetPosition();
			Vector3D dir = -sBoss_->GetFrontVec();
			dir.y = 0.0f;
			float spd = 0.1f;
			pos += dir * spd;
			sBoss_->SetPosition(pos);
		}

		if (timer_.GetIsActive() == false) {
			int rad = rand();
			rad = rad % StateNum;
			if (rad == BulletState) {
				std::unique_ptr<EnemyState> next_ = std::make_unique<BossBulletState>();
				sBoss_->SetCurrentState(next_);
			}
			else if (rad == WayBulletsState) {
				std::unique_ptr<EnemyState> next_ = std::make_unique<BossWayBullets>();
				sBoss_->SetCurrentState(next_);
			}
		}

		//InputManager* input = InputManager::GetInstance();
		//if (input->GetKeyAndButton(DIK_1, InputJoypad::A_Button)) {
		//	std::unique_ptr<EnemyState> next_ = std::make_unique<BossBulletState>();
		//	sBoss_->SetCurrentState(next_);
		//}
		////if (input->GetKeyAndButton(DIK_2, InputJoypad::A_Button)) {
		////	std::unique_ptr<EnemyState> next_ = std::make_unique<BossMeleeState>();
		////	sBoss_->SetCurrentState(next_);
		////}
		//if (input->GetKeyAndButton(DIK_2, InputJoypad::A_Button)) {
		//	std::unique_ptr<EnemyState> next_ = std::make_unique<BossWayBullets>();
		//	sBoss_->SetCurrentState(next_);
		//}
	}
}
