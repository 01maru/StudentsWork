#pragma once
#include "Vector2D.h"
#include "ICamera.h"

class InputMouse;
class InputKeyboard;

class MyDebugCamera :public ICamera
{
private:
	const float MIN_EYE_TO_TARGET = 1.0f;

	InputMouse* mouse_ = nullptr;
	InputKeyboard* keyboard_ = nullptr;

	Vector2D rotValue_;

	enum MoveMode {
		TranslationMove,
		RotationMove,
	};
	MoveMode mode_ = TranslationMove;

private:
	void CalcDisEyeToTarget();
	void SetMoveMode(bool active);
	Vector3D CalcTransMove(bool active);
	void CalcRotMove(bool active);
	void SetPosition(const Vector3D& moveTarget);

	void ImGuiInfo() override;

public:
	MyDebugCamera() {};
	~MyDebugCamera() override {};

	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

