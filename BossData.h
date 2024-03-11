#pragma once
#include <cstdint>
#include "Vector3D.h"

/**
* @file BossData.h
* @brief ボスのステータスやパラメーターを管理するためのファイル
*/

class BossData
{
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

		//	RockFall

public:
#pragma region Getter

	int32_t GetBumpRate();
	int32_t GetBumpLifeTime();
	float GetBumpHeight();
	float GetBumpScale();

#pragma endregion

};

