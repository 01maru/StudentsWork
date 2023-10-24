#include "SpaceShip.h"
#include "Easing.h"
#include "CameraManager.h"


using namespace Easing;

void SpaceShip::Initialize(IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	mat_.scale_ = Vector3D(0.0f, 0.0f, 0.0f);
	counter_.Initialize(scaleMaxFrame_, true);
	mat_.trans_.y = startPosY_;
}

void SpaceShip::ScaleUpdate()
{
	if (scaling_ == true) return;

	float scale = EaseIn(0.2f, maxScale_, counter_.GetCountPerMaxCount(), 2);
	mat_.scale_ = Vector3D(scale, scale, scale);
	float fade = EaseIn(0.0f, 1.0, counter_.GetCountPerMaxCount(), 2);
	color_.w = fade;

	if (counter_.GetFrameCount() == scaleMaxFrame_) {
		scaling_ = true;
		counter_.Initialize(moveMaxFrame_, true);
		counter_.StartCount();
	}
}

void SpaceShip::MoveUpdate()
{
	if (scaling_ == false) return;
	if (moving_ == true) return;

	float posY = EaseIn(startPosY_, -0.3f, counter_.GetCountPerMaxCount(), 5);
	mat_.trans_.y = posY;

	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	float shakeValue= EaseIn(0.0f, 0.5f, counter_.GetCountPerMaxCount(), 2);
	camera->SetShake(-shakeValue, shakeValue);

	if (counter_.GetFrameCount() == moveMaxFrame_) {
		moving_ = true;
		stopShaking_ = true;
		counter_.Initialize(20, true);
		counter_.StartCount();
	}
}

void SpaceShip::CollisionGroundShake()
{
	if (scaling_ == false) return;
	if (moving_ == false) return;
	if (stopShaking_ == false) return;

	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	float shakeValue = EaseOut(0.75f, 0.0f, counter_.GetCountPerMaxCount(), 2);
	camera->SetShake(-shakeValue, shakeValue);

	if (counter_.GetFrameCount() == 20) {
		stopShaking_ = false;
		isEnd_ = true;
		camera->StopShake();
	}
}

void SpaceShip::OpenDoorShake()
{
	if (doorShaking_ == false) return;

	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	float shakeValue = EaseOut(0.1f, 0.0f, counter_.GetCountPerMaxCount(), 2);
	camera->SetShake(-shakeValue, shakeValue);

	if (counter_.GetFrameCount() == 20) {
		doorShaking_ = false;
		cameraMoving_ = true;
		counter_.Initialize(30, true);
		counter_.StartCount();
		camera->StopShake();
	}
}

void SpaceShip::ShakeUpdate()
{
	CollisionGroundShake();

	OpenDoorShake();
}

void SpaceShip::CameraMoveUpdate()
{
	if (cameraMoving_ == false) return;

	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	camera->SetEye(lerp(startEye_, endEye_, counter_.GetCountPerMaxCount()));
	Vector3D target = startTarget_;
	target.y = lerp(startTarget_.y, endTargetY_, counter_.GetCountPerMaxCount());
	camera->SetTarget(target);

	if (counter_.GetFrameCount() == 30) {
		cameraMoving_ = false;
		cameraFrontMove_ = true;
		startEye_ = camera->GetEye();
		startTarget_ = camera->GetTarget();
		counter_.Initialize(30, true);
		counter_.StartCount();
	}
}

void SpaceShip::CameraFrontUpdate()
{
	if (cameraFrontMove_ == false) return;

	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	float len = EaseOutBack(0.0f, moveZ, counter_.GetCountPerMaxCount());
	Vector3D eye = startEye_;
	eye.z += len;
	camera->SetEye(eye);
	Vector3D target = startTarget_;
	target.z = len;
	camera->SetTarget(target);

	if (counter_.GetFrameCount() == 30) {
		cameraFrontMove_ = false;
	}
}

void SpaceShip::Update()
{
	counter_.Update();
	ScaleUpdate();
	MoveUpdate();

	if (isEnd_ == false) {
		ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
		camera->SetTarget(mat_.trans_ + Vector3D(0.0f, 2.0f, 0.0f));
	}
	ShakeUpdate();

	CameraMoveUpdate();
	CameraFrontUpdate();

	Object3D::MatUpdate(0);
}

void SpaceShip::Start()
{
	moving_ = false;
	isEnd_ = false;
	scaling_ = false;
	stopShaking_ = false;
	counter_.Initialize(scaleMaxFrame_, true);
	counter_.StartCount();
	mat_.scale_ = Vector3D(0.2f, 0.2f, 0.2f);
	//mat_.scale_ = Vector3D(maxScale_, maxScale_, maxScale_);
	mat_.trans_.y = startPosY_;
}

void SpaceShip::OpenDoor()
{
	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();
	doorShaking_ = true;
	counter_.Initialize(20, true);
	counter_.StartCount();
}
