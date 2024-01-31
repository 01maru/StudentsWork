#include "CrossHair.h"
#include "CollisionManager.h"
#include "RayCast.h"
#include "CameraManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"

using namespace CollAttribute;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void CrossHair::Update(const Vector3D& startPos)
{
	//	カメラの前方方向にレイを飛ばす
	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	camera->CalcDirectionVec();
	Ray ray;
	ray.start = camera->GetEye();
	ray.dir = camera->GetFrontVec();
	RayCast raycastHit;

	//	弾の発射方向更新
	//	例が敵か地形に当たったら
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ENEMYS | COLLISION_ATTR_LANDSHAPE, &raycastHit)) {
		dir_ = raycastHit.inter - startPos;
	}
	//	当たらなかったら
	else {
		dir_ = camera->GetEye() + camera->GetFrontVec() * MAX_DISTANCE - startPos;
	}
	dir_.Normalize();
	
	//	スプライト更新
	sprite_.Update();
}

void CrossHair::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	//	弾の発射方向
	imgui->Text("CrossHair Dir : (%.2f, %.2f, %.2f)", dir_.x, dir_.y, dir_.z);
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void CrossHair::Draw()
{
	sprite_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector3D CrossHair::GetDir()
{
	return dir_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void CrossHair::SetSprite(const MNE::Sprite& sprite)
{
	sprite_ = sprite;
}
