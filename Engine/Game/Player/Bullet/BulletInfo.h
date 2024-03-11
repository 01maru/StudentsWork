#pragma once
#include "Vector4D.h"
#include "Vector3D.h"
#include "Matrix.h"

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

enum EnemyBulletType
{
	NormalStone,
	BumpStone,
	LandStone,
};

struct EnemyBulletInfo
{
	int32_t type_;
	int32_t lifeTime_;
	float spd_;
	MyMath::Vector3D moveVec_;
	MyMath::Vector3D scale_;
	MyMath::Vector3D pos_;
	MyMath::Vector3D acc_;
};

struct BeamInfo
{
	bool active_;
	MyMath::Vector4D color_;
	MyMath::Vector3D scale_;
	MyMath::Vector3D pos_;
	MyMath::Matrix rotMat_;
};
