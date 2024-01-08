#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"

#pragma region  前置宣言

class ICamera;

#pragma endregion

/**
* @file PodCollGroundState.h
* @brief 脱出ポッドが地面に衝突した際の揺れとドアを開ける入力を管理するファイル
*/

class PodCollGroundState :public EscPodState
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
	//	動かすカメラのポインター
	ICamera* camera = nullptr;
	//	地面の揺れる時間(フレーム数)
	int16_t collGroundShakeFrame_ = 20;
};

