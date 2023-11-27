#include "GameOverCamera.h"
#include "Easing.h"
#include "InputManager.h"
#include "ImGuiManager.h"

#include "SceneManager.h"
#include "GameOverUI.h"

using namespace Easing;

void GameOverCamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	target_ = center;
	disEyeTarget_ = dis;

	Vector3D front = frontVec;
	front.Normalize();

	eye_ = target_ - disEyeTarget_ * front;

	MatUpdate();

}

void GameOverCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	counter_.Initialize(40, true);
	eyeStart_ = eye;
	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	float len = disEyeTarget_ / 5.0f;
	controlPoint_ = eye + frontVec_ * len;
	controlPoint_.y = target.y - 2.0f;
	Vector3D front = frontVec_;
	front.y = 0;
	eyeEnd_ = eye + front * (disEyeTarget_ - 5);
	eyeEnd_.y = target.y;

	//if (InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_RETURN)) {
		counter_.StartCount();
		fadeCounter_.Initialize(60, true);
		GameOverUI::GetInstance()->Start();
	//}
}

void GameOverCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputInt("EaseOutPow", pow1);
}

void GameOverCamera::Update()
{
	if (counter_.GetFrameCount() == counter_.GetMaxFrameCount()&&
		fadeCounter_.GetFrameCount() == 0) {
		fadeCounter_.StartCount();
	}
	fadeCounter_.Update();

	counter_.Update();
	Vector3D a = EaseOut(eyeStart_, controlPoint_, counter_.GetCountPerMaxCount(), pow1);
	Vector3D b = EaseOut(controlPoint_, eyeEnd_, counter_.GetCountPerMaxCount(), pow1);
	eye_ = EaseOut(a, b, counter_.GetCountPerMaxCount(), pow1);

	float color = Easing::lerp(0.0f, 1.0f, fadeCounter_.GetCountPerMaxCount());
	SceneManager::GetInstance()->ChangeScreenAlpha(color);

	ShakeUpdate();

	MatUpdate();
}
