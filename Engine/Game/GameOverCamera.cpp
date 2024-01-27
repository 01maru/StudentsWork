#include "GameOverCamera.h"
#include "Easing.h"
#include "ImGuiManager.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void GameOverCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	//	カウンター初期化&カウントスタート
	counter_.Initialize(moveTime_, true);
	counter_.StartCount();

	//	視点開始位置設定
	eyeStart_ = eye;
	//	イージングの強さ
	easePaw_ = Easing::Quint;

	//	初期化
	ICamera::Initialize(eye, target, up);
}

//-----------------------------------------------------------------------------
// [SECTION] ImGui
//-----------------------------------------------------------------------------

void GameOverCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputInt("EaseOutPow", easePaw_);

	//	制御点
	imgui->InputFloat("cPointYRate", cPointYRate_);
	imgui->InputFloat("cPointLenRate", cPointLenRate_);

	//	終了視点
	imgui->InputFloat("eEyeYRate", eEyeYRate_);
	imgui->InputFloat("eEyeLenRate", eEyeLenRate_);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void GameOverCamera::Update()
{
	//	カウンターが止まっていたら処理しない
	if (counter_.GetIsActive() == FALSE) return;

	counter_.Update();

	//	ベジェ曲線で視点移動
	Vector3D a = EaseOut(eyeStart_, controlPoint_, counter_.GetCountPerMaxCount(), easePaw_);
	Vector3D b = EaseOut(controlPoint_, eyeEnd_, counter_.GetCountPerMaxCount(), easePaw_);
	eye_ = EaseOut(a, b, counter_.GetCountPerMaxCount(), easePaw_);
	//	ターゲットが視点より上にないように
	target_.y = MyMath::mMin(target_.y, eye_.y);

	ICamera::Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool GameOverCamera::GetEndCameraMove()
{
	//	カウンターが止まっていたらTRUEを返す
	return (counter_.GetIsActive() == FALSE);
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void GameOverCamera::Reset()
{
	//	カウントリセット
	counter_.ResetCount();
	//	スタート
	counter_.StartCount();
}

void GameOverCamera::SetPosData(const Vector3D& pos)
{
	Vector3D front = frontVec_;
	front.y = 0;

	//	制御点
	float posY = eyeStart_.y * cPointYRate_;
	float len = disEyeTarget_ * cPointLenRate_;
	controlPoint_ = pos - front * len;
	controlPoint_.y = posY;

	//	終了位置
	posY = eyeStart_.y * eEyeYRate_;
	len = disEyeTarget_*eEyeLenRate_;
	const float MIN_HEIGHT = 2.0f;

	posY = MyMath::mMin(MIN_HEIGHT, posY);
	eyeEnd_ = pos - front * len;
	eyeEnd_.y += posY;
}
