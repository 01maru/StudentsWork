#include "GameOverCamera.h"
#include "Easing.h"
#include "ImGuiManager.h"

#include "SceneManager.h"

using namespace Easing;
using namespace MNE;

void GameOverCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	counter_.Initialize(40, true);
	eyeStart_ = eye;

	ICamera::Initialize(eye, target, up);

	float len = disEyeTarget_ / 5.0f;
	controlPoint_ = eye + frontVec_ * len;
	controlPoint_.y = target.y - 2.0f;
	Vector3D front = frontVec_;
	front.y = 0;
	eyeEnd_ = eye + front * (disEyeTarget_ - 5);
	eyeEnd_.y = target.y;

	counter_.StartCount();
}

void GameOverCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputInt("EaseOutPow", pow1);
}

void GameOverCamera::Update()
{
	counter_.Update();
	Vector3D a = EaseOut(eyeStart_, controlPoint_, counter_.GetCountPerMaxCount(), pow1);
	Vector3D b = EaseOut(controlPoint_, eyeEnd_, counter_.GetCountPerMaxCount(), pow1);
	eye_ = EaseOut(a, b, counter_.GetCountPerMaxCount(), pow1);

	ICamera::Update();
}

bool GameOverCamera::StartFade()
{
	return counter_.GetFrameCount() == counter_.GetMaxFrameCount();
}
