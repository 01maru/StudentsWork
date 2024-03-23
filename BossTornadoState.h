#pragma once
#include "BossState.h"
#include "FrameCounter.h"
#include "SuctionEmptyObj.h"
#include <array>

class BossTornadoState :public BossState
{
private:
	//	ステート
	enum TornadoState
	{
		WaitState = 0,		//	チャージ中
		AttackState,		//	攻撃
		EndState,			//	終了時演出
		StanState,			//	スタン
		TotalStates,
	};

	typedef void (BossTornadoState::* state)();
	//	関数ポインタテーブル
	std::array<state, TotalStates> stateTable_;
	//	現在のステート
	int32_t nowState_ = WaitState;

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
	MNE::FrameCounter timer_;
	SuctionEmptyObj emptyObj_;

private:
	void WaitStateInitialize();
	void WaitStateUpdate();
	void AttackStateInitialize();
	void AttackStateUpdate();
	void EndStateInitialize();
	void EndStateUpdate();
	void StanStateInitialize();
	void StanStateUpdate();
};

