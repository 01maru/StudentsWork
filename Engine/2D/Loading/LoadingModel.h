#pragma once
#include "Object3DSilhouette.h"
#include "ILoadingObj.h"

/**
* @file LoadingModel.h
* @brief ILoadingObjを継承したモデルのローディング表示用処理をまとめたファイル
*/

namespace MNE
{

	class LoadingModel :public ILoadingObj, public Object3DSilhouette
	{
	public:
		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize() override;
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update() override;
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		void Draw() override;

	private:
		int32_t easePaw_ = 4;
		float rotSpd_ = 0.05f;
	};

}
