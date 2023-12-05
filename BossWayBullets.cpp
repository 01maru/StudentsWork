#include "BossWayBullets.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "ModelManager.h"
#include "Quaternion.h"

void BossWayBullets::Initialize()
{
	rate_.Initialize(60, true);
	rate_.StartCount();
}

void BossWayBullets::Update()
{
	sBoss_->RotationUpdate();

	rate_.Update();

	if (rate_.GetIsActive() == false) {

		for (size_t i = 0; i < bulletMaxNum_; i++)
		{
 			Vector3D dirVec = -sBoss_->GetFrontVec();
			float angle = MyMath::ConvertToRad(15.0f);
			if (i > 2) {
				angle *= (i-2);
				angle = -angle;
			}
			else {
				angle *= i;
			}
			Quaternion rot = SetQuaternion(Vector3D(0, 1, 0), angle);
			dirVec = RotateVector(dirVec, rot);

			//	弾生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize();
			bullet->SetLifeTime(600);
			bullet->SetSpd(1);
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
