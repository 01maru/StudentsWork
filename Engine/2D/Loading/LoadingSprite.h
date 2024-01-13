#pragma once
#include "ILoadingObj.h"
#include "Sprite.h"

/**
* @file LoadingSprite.h
* @brief ILoadingObjを継承したスプライトのローディング表示用処理をまとめたファイル
*/

namespace MNE
{

	class LoadingSprite :public ILoadingObj, public Sprite
	{
	public:
		~LoadingSprite() override {}

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
		int32_t sizeEasePaw_ = 2;
		FrameCounter timer_;

		float minSize_ = 10.0f;
		float maxSize_ = 30.0f;
		Vector3D color_ = { 0.2f,0.2f,0.2f };

	public:
		void Start();
		void Reset();

		void SetMaxSize(float size);
	};

}
