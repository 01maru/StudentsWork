#pragma once
#include "Vector2D.h"
#include "ICamera.h"

class MyDebugCamera :public ICamera
{
private:
	float disEyeTarget_ = 0.0f;

	Vector2D rotValue_;

	enum MoveMode {
		TranslationMove,
		RotationMove,
	};
	MoveMode mode_ = TranslationMove;

private:
	void CalcDisEyeToTarget();
	void SetMoveMode(bool active);
	Vector3D CalcMoveTarget(bool active, Vector2D& moveCursor);
	void CalcRotValue(bool active, Vector2D& moveCursor);
	void SetPosition(const Vector3D& moveTarget);
public:
	MyDebugCamera() {};
	~MyDebugCamera() override {};

	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

