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
	Vector3D moveVec_;
	Vector3D pos_;
};
