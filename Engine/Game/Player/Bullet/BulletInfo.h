#pragma once
#include "Vector3D.h"

/**
* @file BulletInfo.h
* @brief プレイヤーの弾丸情報を弾マネージャーに送る用の構造体のファイル
*/

struct BulletInfo
{
	int32_t lifeTime_;
	float spd_;
	MyMath::Vector3D moveVec_;
	MyMath::Vector3D pos_;
};

struct EnemyBulletInfo
{
	int32_t lifeTime_;
	float spd_;
	MyMath::Vector3D moveVec_;
	MyMath::Vector3D pos_;
	MyMath::Vector3D acc_;
};
