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
	MNE::FrameCounter counter_;
	//	このState終了までのフレーム数
	int16_t moveMaxFrame_ = 100;
	//	ポッドの位置
	float startPosY_ = 100.0f;	//	開始
	float endPosY_;				//	終了
	MyMath::Vector3D targetToPod_;		//	ポッド座標とターゲットのオフセット値
	//	カメラのシェイクマックス値
	float maxShakeV_ = 0.2f;
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;
};

