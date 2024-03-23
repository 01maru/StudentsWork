#pragma once
#include "BulletInfo.h"
#include "BossState.h"
#include "FrameCounter.h"
#include <array>

/**
* @file BossBeamState.h
* @brief ボスのビーム攻撃の動きを管理するファイル
*/

class BossBeamState :public BossState
{
private:
	//	ステート
	enum BeamState
	{
		ChargeState = 0,	//	チャージ中
		AttackState,		//	攻撃
		EndState,			//	終了時演出
		TotalStates,
	};

	typedef void (BossBeamState::* state)(BeamInfo&);
	//	関数ポインタテーブル
	std::array<state, TotalStates> stateTable_;
	//	現在のステート
	int32_t nowState_ = ChargeState;

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

private:
	bool attacking_;

	MNE::FrameCounter timer_;
	MNE::FrameCounter avoidTimer_;

	MyMath::Vector3D chargeScale_ = { 0.5f, 0.5f, 0.5f };
	MyMath::Vector3D maxScale_ = { 0.8f, 0.8f, 0.8f };

	MyMath::Vector3D color_ = { 0.7f, 0.1f, 0.1f };
	float chargeAlpha_ = 0.4f;

private:
	void RayCollisionUpdate(BeamInfo& info);
	bool AvoidTimerUpdate();
	void ChargeStateInitialize();
	void ChargeStateUpdate(BeamInfo& info);
	void AttackStateInitialize();
	void AttackStateUpdate(BeamInfo& info);
	void EndStateInitialize();
	void EndStateUpdate(BeamInfo& info);
};

