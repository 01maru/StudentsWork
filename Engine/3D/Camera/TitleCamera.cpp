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
	//	移動中じゃなかったら処理しない
	if (counter_.GetIsActive() == false) return;

	//	視点移動
	float t = EaseOut(0.0f, 1.0f, counter_.GetCountPerMaxCount(), Easing::Single);
	eye_ = MyMath::BezierCurve(startEye_, controlPoint_, endEye_, t);
	frontVec_ = MyMath::BezierCurve(startFront_, controlPoint_, endFront_, t);
	frontVec_.Normalize();

	//	ターゲット移動
	target_ = eye_ + frontVec_ * disEyeTarget_;
}
void TitleCamera::Update()
{
	//	カウンター更新
	counter_.Update();

	CameraMoveUpdate();

	ICamera::Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool TitleCamera::GetIsMoving()
{
	return counter_.GetIsActive();
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
	//	モード設定
	mode_ = mode;

	//	開始位置格納
	startEye_ = eye_;
	startFront_ = frontVec_;

	//	終了時の値格納
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

	//	カウンタースタート
	counter_.StartCount();
}
