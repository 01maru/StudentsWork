#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file PodOpenDoorState.h
* @brief 脱出ポッドのドアを開ける演出とカメラの動きを管理するファイル
*/

#pragma region  前置宣言
namespace MNE
{
	class ICamera;
}
#pragma endregion

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
	int16_t openDoorFrame_ = 10;	//	カウントマックス値
	//	開始位置
	Vector3D startEye_;
	Vector3D startTarget_;
	//	カメラの移動量
	Vector3D move_;
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;

};

