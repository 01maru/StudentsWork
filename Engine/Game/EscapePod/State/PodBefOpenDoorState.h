#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file PodBefOpenDoorState.h
* @brief 脱出ポッドのドアを開ける前のカメラの動きを管理するファイル
*/

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
	MNE::FrameCounter counter_;
	int16_t cameraMoveFrame_ = 90;
	//	開始視点
	MyMath::Vector3D startEye_;
	//	終了視点
	MyMath::Vector3D endEye_ = MyMath::Vector3D(0.0f, 1.0f, -64.0f);
	//	開始ターゲット位置
	MyMath::Vector3D startTarget_;
	//	終了時ターゲットの高さ
	float endTargetY_ = 1.7f;
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;

	//	カメラシェイク
	int16_t shakeFrame_ = 20;	//	時間
	float maxShakeV_ = 0.1f;	//	シェイクのマックス値

private:
	/**
	* @fn ShakeUpdate()
	* カメラシェイクの更新処理関数
	*/
	void ShakeUpdate();
	/**
	* @fn CameraMoveUpdate()
	* カメラ移動の更新処理関数
	*/
	void CameraMoveUpdate();
};

