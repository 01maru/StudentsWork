#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"

/**
* @file PodCollGroundState.h
* @brief 脱出ポッドが地面に衝突した際の揺れとドアを開ける入力を管理するファイル
*/

#pragma region  前置宣言
namespace MNE
{
	class ICamera;
}
#pragma endregion

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
	MNE::ICamera* camera = nullptr;
	//	地面の揺れる時間(フレーム数)
	int16_t collGroundShakeFrame_ = 20;
	//	カメラシェイクのマックス値
	float maxShakeV_ = 0.75f;
};

