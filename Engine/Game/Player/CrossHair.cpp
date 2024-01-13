#include "CrossHair.h"
#include "CollisionManager.h"
#include "RayCast.h"
#include "CameraManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"

using namespace CollAttribute;
using namespace MNE;

void CrossHair::Update(const Vector3D& startPos)
{
	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	camera->CalcDirectionVec();
	Ray ray;
	ray.start = camera->GetEye();
	ray.dir = camera->GetFrontVec();
	RayCast raycastHit;

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ENEMYS | COLLISION_ATTR_LANDSHAPE, &raycastHit)) {
		dir_ = raycastHit.inter - startPos;
	}
	else {
		dir_ = camera->GetEye() + camera->GetFrontVec() * MAX_DISTANCE - startPos;
	}
	dir_.Normalize();
	
	sprite_.Update();
}

void CrossHair::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("CrossHair Dir : (%.2f, %.2f, %.2f)", dir_.x, dir_.y, dir_.z);
}

void CrossHair::Draw()
{
	sprite_.Draw();
}
