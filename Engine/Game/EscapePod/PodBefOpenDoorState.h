#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

#pragma region  前置宣言

namespace MNE
{
	
	class ICamera;

}

#pragma endregion

class PodBefOpenDoorState :public EscPodState
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	//	アニメーション用カウンター
	FrameCounter counter_;
	Vector3D startEye_;
	Vector3D endEye_ = Vector3D(0.0f, 1.0f, -64.0f);
	Vector3D startTarget_;
	float endTargetY_ = 1.7f;
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;

	int16_t cameraMoveFrame_ = 90;

	int16_t shakeFrame_ = 20;
	float maxShakeV_ = 0.1f;

private:
	void ShakeUpdate();
	void CameraMoveUpdate();
};

