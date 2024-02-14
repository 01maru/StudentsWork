#pragma once
#include "CollisionType.h"
#include "CollisionInfo.h"
#include <cstdint>
#include "Matrix.h"
#include "Object3D.h"

/**
* @file BaseCollider.h
* @brief 当たり判定の基盤となるファイル
*/

class BaseCollider
{
public:
	friend class CollisionManager;

	//	コンストラクタ
	BaseCollider() = default;
	//	デストラクタ
	virtual ~BaseCollider() = default;

	/**
	* @fn Finalize()
	* 終了処理関数
	*/
	void Finalize();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() = 0;
	inline void OnCollision(CollisionInfo& info) { object_->OnCollision(info); }

protected:
	//	コライダーをリストから削除するかフラグ
	bool deleteFlag_ = false;

	//	親オブジェクト
	MNE::Object3D* object_ = nullptr;
	//	親ワールド行列(設定しなかったらオブジェクトを使用)
	MyMath::Matrix* matWorld_ = nullptr;

	//	当たり判定の型
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	//	属性
	uint16_t attribute_ = 0b1111111111111111;

public:
#pragma region Getter

	MyMath::Matrix GetWorldMatrix();
	inline MNE::Object3D* GetObject3D() { return object_; }
	inline CollisionShapeType GetShapeType() { return shapeType_; }
	bool GetDeleteFlag() { return deleteFlag_; }
	uint16_t GetAttribute() { return attribute_; }

#pragma endregion

#pragma region Setter

	inline void SetObject3D(MNE::Object3D* object) { object_ = object; }
	inline void SetAttribute(unsigned short attribute) { attribute_ = attribute; }
	inline void AddAttribute(unsigned short attribute) { attribute_ |= attribute; }
	inline void RemoveAttribute(unsigned short attribute) { attribute_ &= !attribute; }

#pragma endregion
};

