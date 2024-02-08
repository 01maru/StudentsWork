#pragma once
#include "ICamera.h"
#include "ConstBuff.h"

/**
* @file LightCamera.h
* @brief shadowMap生成に必要な方向ライト用のカメラファイル
*/

namespace MNE
{

	namespace CBuff {
		struct CBuffLightMaterial;
	}

	class LightCamera :public MNE::ICamera
	{
	private:
		MNE::ConstBuff lightMaterial_;
		MNE::CBuff::CBuffLightMaterial* cLightMap_ = nullptr;

	private:
		void ConstBuffInitialize();

	public:
		/**
		* @fn Initialize(const Vector3D&, const Vector3D&, float)
		* 初期化用関数
		* @param frontVec 前方方向ベクトル
		* @param center 注視点座標
		* @param dis 注視点と視点座標の距離
		*/
		void Initialize(const MyMath::Vector3D& frontVec, const MyMath::Vector3D& center, float dis) override;
		/**
		* @fn Initialize(const Vector3D&, const Vector3D&, const Vector3D&)
		* 初期化用関数
		* @param eye 視点座標
		* @param target 注視点座標
		* @param up 上方向ベクトル
		*/
		void Initialize(const MyMath::Vector3D& eye, const MyMath::Vector3D& target, const MyMath::Vector3D& up) override;
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update() override;

		void SetGraphicsRootCBuffView(uint32_t rootParaIdx);
	};

}
