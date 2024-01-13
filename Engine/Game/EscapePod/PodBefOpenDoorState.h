#pragma once
#include "EscPodState.h"
#include "FrameCounter.h"

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
	//	動かすカメラのポインター
	MNE::ICamera* camera = nullptr;

	int16_t shakeFrame_ = 20;
};

