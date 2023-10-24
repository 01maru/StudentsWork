#pragma once
#include "Object3D.h"
#include "FrameCounter.h"

class SpaceShip :public Object3D
{
private:
	bool isEnd_ = true;
	bool scaling_ = false;
	int16_t scaleMaxFrame_ = 20;
	float maxScale_ = 0.5f;
	bool moving_ = false;
	int16_t moveMaxFrame_ = 100;
	float startPosY_ = 100.0f;
	bool stopShaking_ = false;

	bool doorShaking_ = false;
	bool cameraMoving_ = false;
	Vector3D startEye_;
	Vector3D endEye_ = Vector3D(0.0f, 4.0f, -14.0f);
	Vector3D startTarget_;
	float endTargetY_ = 1.7f;
	FrameCounter counter_;

	bool cameraFrontMove_ = false;
	float moveZ = 7.0f;
	
private:
	void ScaleUpdate();
	void MoveUpdate();
	void CollisionGroundShake();
	void OpenDoorShake();
	void ShakeUpdate();
	void CameraMoveUpdate();
	void CameraFrontUpdate();
public:
	void Initialize(IModel* model);
	void Update();
	void Start();
	void OpenDoor();

	bool GetIsEnd() { return isEnd_; }
	void SetIsEnd(bool isEnd) { isEnd_ = isEnd; }
};

