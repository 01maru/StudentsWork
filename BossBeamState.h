#pragma once
#include "BossState.h"
#include "FrameCounter.h"
#include "Object3D.h"

/**
* @file BossBeamState.h
* @brief ボスのビーム攻撃の動きを管理するファイル
*/

class BossBeamState :public BossState
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

private:

	MNE::Object3D beamObj_;

	MNE::FrameCounter rate_;
	int32_t delayTime_ = 60;
	float bulletSpd_ = 1.0f;

};

