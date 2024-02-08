#pragma once
#include "Object3D.h"
#include "FrameCounter.h"
#include "ParticleEmitter.h"

class Bullet :public MNE::Object3D
{
public:
	//	デストラクタ
	~Bullet();

	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();

private:
	const int32_t LIFE_TIME = 600;
	//	移動方向ベクトル
	MyMath::Vector3D moveVec_;
	//	移動速度
	float spd_;
	//	生存時間管理用タイマー
	MNE::FrameCounter lifeTime_;
	//	半径
	float radius_ = 0.4f;
	//	ヒット時のダメージ
	int32_t damage_ = 5;
	//	エミッターの生存時間
	int32_t emitterLifeTime_ = 30;
	MNE::ParticleEmitter* emitter_ = nullptr;

public:
	void OnCollision(CollisionInfo& info) override;

#pragma region Getter

	/**
	* @fn GetIsActive()
	* アクティブかのフラグ取得用関数
	* @return アクティブかどうか
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
	* 弾の速度設定用関数
	* @param spd 弾の速度
	*/
	void SetSpd(float spd);
	/**
	* @fn SetLifeTime(int32_t)
	* 弾の生存時間設定用関数
	* @param time 弾の生存時間
	*/
	void SetLifeTime(int32_t time);

#pragma endregion
};

