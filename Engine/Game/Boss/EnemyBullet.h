#pragma once
#include "Object3D.h"
#include "FrameCounter.h"

/**
* @file EnemyBullet.h
* @brief ボスの弾を管理するファイル
*/

class EnemyBullet :public MNE::Object3D
{
public:
	//	デストラクタ
	~EnemyBullet() {};
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();

private:
	const int32_t LIFE_TIME = 600;
	MyMath::Vector3D moveVec_;
	float spd_;
	MNE::FrameCounter lifeTime_;
	float radius_ = 0.4f;
	int32_t damage_ = 5;

public:
	void OnCollision(CollisionInfo& info) override;

#pragma region Getter

	/**
	* @fn GetIsActive()
	* アクティブか取得用関数
	* @return アクティブか
	*/
	bool GetIsActive();
	
#pragma endregion

#pragma region Setter

	/**
	* @fn SetMoveVec(const Vector3D&)
	* 移動方向ベクトル設定用関数
	* @param moveVec 移動方向ベクトル
	*/
	void SetMoveVec(const MyMath::Vector3D& moveVec);
	/**
	* @fn SetSpd(float)
	* 速度設定用関数
	* @param spd 弾の速度
	*/
	void SetSpd(float spd);
	/**
	* @fn SetLifeTime(int32_t)
	* 生存時間設定用関数
	* @param time 生存時間
	*/
	void SetLifeTime(int32_t time);

#pragma endregion
};
