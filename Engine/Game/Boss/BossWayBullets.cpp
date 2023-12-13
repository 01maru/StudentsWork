#include "BossWayBullets.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "ModelManager.h"
#include "Quaternion.h"
#include "BossDeathState.h"

void BossWayBullets::Initialize()
{
	rate_.Initialize(delayTime_, true);
	rate_.StartCount();

	float angle = 10.0f;
	wayAngle_ = MyMath::ConvertToRad(angle);
}

void BossWayBullets::Update()
{
	if (sBoss_->GetIsAlive() == false) {
		std::unique_ptr<EnemyState> next_ = std::make_unique<BossDeathState>();
		sBoss_->SetCurrentState(next_);
	}

	sBoss_->RotationUpdate();

	rate_.Update();

	if (rate_.GetIsActive() == false) {

		float startAngle;
		int32_t half = 2;
		int32_t bulletHalfNum = bulletMaxNum_ / half;
		//	偶数だったら
		bool isEvenNumver = bulletMaxNum_ % half == 0;
		if (isEvenNumver == true) {
			startAngle = wayAngle_ * bulletHalfNum - wayAngle_ / static_cast<float>(half);
		}
		else {
			startAngle = wayAngle_ * bulletHalfNum;
		}

		for (size_t i = 0; i < bulletMaxNum_; i++)
		{
 			Vector3D dirVec = -sBoss_->GetFrontVec();
 			float angle = wayAngle_ * i;

			angle = startAngle - angle;

			Vector3D up(0, 1, 0);
			Quaternion rot = SetQuaternion(up, angle);
			dirVec = RotateVector(dirVec, rot);

			//	弾生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize();
			bullet->SetLifeTime(bulletLifeTime_);
			bullet->SetSpd(bulletSpd_);
			bullet->SetMoveVec(dirVec);
			bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
			bullet->SetPosition(sBoss_->GetPosition());
			sBoss_->AddBullet(bullet);
		}

		//	終了
		std::unique_ptr<EnemyState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}
