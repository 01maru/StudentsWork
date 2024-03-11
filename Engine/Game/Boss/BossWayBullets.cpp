#include "BossWayBullets.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "ModelManager.h"
#include "Quaternion.h"

#include "Player.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossWayBullets::Initialize()
{
	rate_.Initialize(delayTime_, true);
	rate_.StartCount();

	float angle = 5.0f;
	wayAngle_ = MyMath::ConvertToRad(angle);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossWayBullets::Update()
{
	sBoss_->RotationUpdate();

	rate_.Update();

	//	弾生成
	if (rate_.GetIsActive() == FALSE) {

		Vector3D pos = sBoss_->GetShotPoint();
		float angle_ = 20.0f;
		float radius_ = 0.1f;

		for (int32_t i = 0; i < bulletMaxNum_; i++)
		{
			float angleXZ = GetRand(0.0f, PIx2);
			float lenRate = GetRand(0.0f, 1.0f);
			float angle = ConvertToRad(angle_ * lenRate - 90.0f);

			Vector3D dirXZ = Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));

			dirXZ.Normalize();
			Vector3D up(0, 1, 0);
			Vector3D v = up.cross(dirXZ);

			Quaternion q = SetQuaternion(v, angle);
			Vector3D dir = RotateVector(dirXZ, q);
			dir.Normalize();

			Vector3D dirVec = sBoss_->GetPlayerPtr()->GetCenterPos() - pos;
			dirVec.Normalize();
			Quaternion rotQ = DirectionToDirection(up, dirVec);

			dir = Vec3Transform(dir, rotQ.GetRotMatrix());
			dirXZ = Vec3Transform(dirXZ, rotQ.GetRotMatrix());
			//Vector3D pos_ = parent_->GetPosition() + radius_ * lenRate * dirXZ;

			//	弾生成
			EnemyBulletInfo bullet;
			bullet.type_ = NormalStone;
			bullet.lifeTime_ = bulletLifeTime_;
			bullet.spd_ = bulletSpd_;

			bullet.moveVec_ = dir;

			bullet.pos_ = pos + radius_ * lenRate * dirXZ;
			sBoss_->AddBullet(bullet);
		}
		//float startAngle;
		//int32_t half = 2;
		//int32_t bulletHalfNum = bulletMaxNum_ / half;
		////	偶数だったら
		//bool isEvenNumber = bulletMaxNum_ % half == 0;
		//if (isEvenNumber == TRUE) {
		//	startAngle = wayAngle_ * bulletHalfNum - wayAngle_ / static_cast<float>(half);
		//}
		//else {
		//	startAngle = wayAngle_ * bulletHalfNum;
		//}

		//for (size_t i = 0; i < bulletMaxNum_; i++)
		//{
 	//		Vector3D dirVec = sBoss_->GetPlayerPtr()->GetCenterPos() - pos;
		//	dirVec.Normalize();
 	//		float angle = wayAngle_ * i;

		//	angle = startAngle - angle;

		//	Vector3D up(0, 1, 0);
		//	Quaternion rot = SetQuaternion(up, angle);
		//	dirVec = RotateVector(dirVec, rot);

		//	//	弾生成
		//	EnemyBulletInfo bullet;
		//	bullet.lifeTime_ = bulletLifeTime_;
		//	bullet.spd_ = bulletSpd_;

		//	bullet.moveVec_ = dirVec;

		//	bullet.pos_ = pos;
		//	sBoss_->AddBullet(bullet);
		//}

		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}
