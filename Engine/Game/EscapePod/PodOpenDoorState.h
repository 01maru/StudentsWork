#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

#pragma region  前置宣言

class ICamera;

#pragma endregion

/**
* @file PodOpenDoorState.h
* @brief 脱出ポッドのドアを開ける演出とカメラの動きを管理するファイル
*/

class PodOpenDoorState :public EscPodState
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
	Vector3D startTarget_;
	float moveZ = 7.0f;
	//	動かすカメラのポインター
	ICamera* camera = nullptr;

	int16_t openDoorFrame_ = 30;
};
