#pragma once
#include <cstdint>
#include <array>
#include "Vector3D.h"

/**
* @file BossData.h
* @brief ボスのステータスやパラメーターを管理するためのファイル
*/

class BossData
{
public:
	enum BossAtState {
		NoAtState = -1,
		BulletState = 0,
		WayBulletsState,
		BeamState,
		JumpAtState,
		RockFallState,
		BumpState,
		Tornado,
		StateNum,
	};

	enum BossForm
	{
		FirstForm = 0,
		SecondForm,
		TotalForm,
	};

	struct AttackPriority
	{
		enum Range
		{
			NowPriority = 0,
			Close,
			Medium,
			Long,
			TotalRange,
		};

		std::array<int32_t, TotalRange> range;
	};

private:
	struct PriorityData
	{
		std::array<std::array<AttackPriority, TotalForm>, StateNum> atState;
	};

public:
	/**
	* @fn SaveData()
	* データをファイルに保存するための関数
	*/
	void SaveData();
	/**
	* @fn LoadData()
	* データをファイルから読み込むための関数
	*/
	void LoadData();

protected:
	//	HP
	int32_t maxHP_ = 100;

	//	Move
	int32_t lenMin_;	//	プレイヤーとの最小距離
	int32_t lenMax_;	//	プレイヤーとの最大距離
	float moveSpd_ = 0.1f;

	//	Attack
		//	BumpAttack
	int32_t bumpRateTime_ = 60;
	int32_t bumpLifeTime_ = 30;
	float bumpHeight_ = 5.0f;
	float bumpScale_ = 2.0f;
		//	Bullet
	int32_t bulletRateTime_ = 60;
	int32_t bulletMaxNum_ = 5;
	float bulletSpd_ = 1.0f;
			//	WayBullet
	int32_t splitBulletMaxNum_ = 8;
	float bulletWayAngle_;
		//	Jump

		//	Beam
	//	攻撃予備動作
	int32_t beamChargeTime_ = 60;
	//	攻撃時間
	int32_t beamAttackTime_ = 60;
	//	攻撃に当たっていない時の経過時間(経過したら攻撃終了)
	int32_t beamAvoidTime_ = 60;
	//	終了演出時間
	int32_t beamEndTime_ = 60;

		//	RockFall
	int32_t rockWaitTime_ = 60;
		//	Tornado
	int32_t tornadoWaitTime_ = 60;
	int32_t tornadoAtTime_ = 240;
	int32_t tornadoEndTime_ = 60;
	int32_t tornadoStanTime_ = 60;
	float tornadoMaxR_ = 70.0f;
	float tornadoRotSpd_ = 0.5f;
	float suctionV_ = 0.1f;

	//	AtPriority
	PriorityData priority_;
	int32_t maxConsecutiveNum_ = 3;

public:
#pragma region Getter

	//	Move
	int32_t GetLenMin();
	int32_t GetLenMax();
	float GetMoveSpd();

	//	Bump
	int32_t GetBumpRate();
	int32_t GetBumpLifeTime();
	float GetBumpHeight();
	float GetBumpScale();

	//	Bullet
	int32_t GetBulletRate();
	int32_t GetBulletMaxNum();
	float GetBulletSpd();
	//	WayBullet
	int32_t GetBulletSplitNum();
	float GetBulletAngle();

	//	Jump

	//	Beam
	int32_t GetBeamChargeTime();
	int32_t GetBeamAttackTime();
	int32_t GetBeamAvoidTime();
	int32_t GetBeamEndTime();

	//	Rock
	int32_t GetRockWaitTime();

	//	Tornado
	int32_t GetTornadoWaitTime();
	int32_t GetTornadoAtTime();
	int32_t GetTornadoEndTime();
	int32_t GetTornadoStanTime();
	float GetTornadoMaxR();
	float GetTornadoRotSpd();
	float GetSuctionValue();

#pragma endregion

};

