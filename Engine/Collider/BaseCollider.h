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

namespace MNE
{
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
		/**
		* @fn OnCollision(CollisionInfo&)
		* 当たった際に処理を行う関数
		* @param info 当たったコライダーの情報
		*/
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

		/**
		* @fn GetWorldMatrix()
		* 親のワールド行列取得用関数
		* @return 親のワールド行列
		*/
		MyMath::Matrix GetWorldMatrix();
		/**
		* @fn GetObject3D()
		* 親オブジェクト取得用関数
		* @return 親オブジェクト
		*/
		inline MNE::Object3D* GetObject3D() { return object_; }
		/**
		* @fn GetShapeType()
		* コライダーの型取得用関数
		* @return コライダーの型
		*/
		inline CollisionShapeType GetShapeType() { return shapeType_; }
		/**
		* @fn GetDeleteFlag()
		* コライダー削除フラグ取得用関数
		* @return コライダー削除フラグ
		*/
		bool GetDeleteFlag();
		/**
		* @fn GetAttribute()
		* 属性取得用関数
		* @return 属性のタグ
		*/
		uint16_t GetAttribute() { return attribute_; }

#pragma endregion

#pragma region Setter

		/**
		* @fn SetObject3D(MNE::Object3D*)
		* 親オブジェクト設定用関数
		* @param object 親のオブジェクト
		*/
		inline void SetObject3D(MNE::Object3D* object) { object_ = object; }
		/**
		* @fn SetAttribute(uint16_t)
		* 属性設定用関数
		* @param attribute 設定する属性
		*/
		inline void SetAttribute(uint16_t attribute) { attribute_ = attribute; }
		/**
		* @fn AddAttribute(uint16_t)
		* 属性追加用関数
		* @param attribute 追加する属性
		*/
		inline void AddAttribute(uint16_t attribute) { attribute_ |= attribute; }
		/**
		* @fn RemoveAttribute(uint16_t)
		* 属性除外用関数
		* @param attribute 除外する属性
		*/
		inline void RemoveAttribute(uint16_t attribute) { attribute_ &= !attribute; }

#pragma endregion
	};
}

