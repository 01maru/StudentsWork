#include "BossBeamState.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "RayCast.h"

#include "BossIdleState.h"
#include "Boss.h"
#include "Player.h"
#include "Quaternion.h"

#include "ModelManager.h"

using namespace MNE;
using namespace MNE::CollAttribute;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossBeamState::Initialize()
{
	beamObj_.Initialize();
	beamObj_.SetModel(ModelManager::GetInstance()->GetModel());
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossBeamState::Update()
{
	Vector3D pos = sBoss_->GetBeamPoint();
	Ray ray;
	ray.start = pos;
	Vector3D dirVec = sBoss_->GetPlayerPtr()->GetCenterPos() - pos;
	dirVec.Normalize();
	ray.dir = dirVec;
	RayCast rayCastHit;
	Vector3D frontVec(0, 0, -1);

	if (CollisionManager::GetInstance()->Raycast(ray,
		COLLISION_ATTR_LANDSHAPE | COLLISION_ATTR_ALLIES, &rayCastHit))
	{
		beamObj_.SetPosition(pos);
		Vector3D scale = beamObj_.GetScale();
		scale.z = rayCastHit.distance;
		beamObj_.SetScale(scale);

		Quaternion rotQ = DirectionToDirection(frontVec, dirVec);
		beamObj_.SetRotMatrix(rotQ.GetRotMatrix());
	}
	//	離れていたら地面についていない
	else {
		////	終了
		//std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		//sBoss_->SetCurrentState(next_);
	}
	
	beamObj_.MatUpdate(FALSE);
}

void BossBeamState::Draw()
{
	beamObj_.Draw();
}
