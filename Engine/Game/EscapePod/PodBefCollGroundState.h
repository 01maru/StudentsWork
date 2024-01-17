#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file PodBefCollGroundState.h
* @brief 脱出ポッドが地面に衝突するまでのポッドとカメラの動きを管理するファイル
*/

#pragma region  前置宣言

namespace MNE
{

	class ICamera;

}

#pragma endregion

class PodBefCollGroundState :public EscPodState
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
	//	このState終了までのフレーム数
	int16_t moveMaxFrame_ = 100;
	//	ポッドの開始位置
	float startPosY_ = 100.0f;
	Vector3D targetToPod_;

	float maxShakeV_ = 0.2f;
	float endPosY_;
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;
};

