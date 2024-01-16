#include "TitleCamera.h"
#include "Easing.h"

using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void TitleCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	//	カメラの初期化
	ICamera::Initialize(eye, target, up);
	//	初期モード設定
	mode_ = Menu;
	//	初期モードの位置設定
	SetCameraPos(eye, target, mode_);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void TitleCamera::CameraMoveUpdate()
{
	if (moveCamera_ == false) return;

	float t = EaseOut(0.0f, 1.0f, counter_.GetCountPerMaxCount(), 1);
	eye_ = MyMath::BezierCurve(startEye_, controlPoint_, endEye_, t);
	frontVec_ = MyMath::BezierCurve(startFront_, controlPoint_, endFront_, t);
	frontVec_.Normalize();

	target_ = eye_ + frontVec_ * disEyeTarget_;

	if (counter_.GetIsActive() == false)
	{
		moveCamera_ = false;
	}
}
void TitleCamera::Update()
{
	counter_.Update();

	CameraMoveUpdate();

	ICamera::Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool TitleCamera::GetIsCameraMove()
{
	return moveCamera_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void TitleCamera::SetAnimationTime(int32_t time)
{
	counter_.Initialize(time, true);
}

void TitleCamera::SetCameraPos(const Vector3D& eye, const Vector3D& target, CameraMode mode)
{
	switch (mode)
	{
	case TitleCamera::Menu:
		titleEye_ = eye;
		titleFront_ = Vector3D(target - titleEye_).GetNormalize();
		break;
	case TitleCamera::Option:
		optionEye_ = eye;
		optionFront_ = Vector3D(target - optionEye_).GetNormalize();
		break;
	case TitleCamera::SceneChange:
		sceneChangeEye_ = eye;
		sceneChangeFront_ = Vector3D(target - sceneChangeEye_).GetNormalize();
		break;
	default:
		break;
	}
}

void TitleCamera::SetNextMode(CameraMode mode)
{
	mode_ = mode;

	startEye_ = eye_;

	startFront_ = frontVec_;

	Vector3D nextEye;
	Vector3D nextFront;

	switch (mode)
	{
	case TitleCamera::Menu:
		nextEye = titleEye_;
		nextFront = titleFront_;
		break;
	case TitleCamera::Option:
		nextEye = optionEye_;
		nextFront = optionFront_;
		break;
	case TitleCamera::SceneChange:
		nextEye = sceneChangeEye_;
		nextFront = sceneChangeFront_;
		break;
	default:
		break;
	}

	endFront_ = nextFront;

	endEye_ = nextEye;

	counter_.StartCount();
	moveCamera_ = true;
}
