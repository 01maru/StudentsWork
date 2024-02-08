#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file PodCameraMoveState.h
* @brief 脱出ポッドのドアが開く前のカメラの動きを管理するファイル
*/

#pragma region  前置宣言
namespace MNE
{
	class ICamera;
}
#pragma endregion

class PodCameraMoveState :public EscPodState
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
	int16_t cameraMoveFrame_ = 30;	//	カウントマックス値
	//	視点の開始と終了時の位置
	MyMath::Vector3D startEye_;
	MyMath::Vector3D endEye_ = MyMath::Vector3D(0.0f, 1.5f, -59.0f);
	//	ターゲットの開始と終了時の位置
	MyMath::Vector3D startTarget_;
	MyMath::Vector3D endTarget_ = MyMath::Vector3D(0.0f, 1.5f, -49.0f);
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;
};

