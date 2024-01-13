#pragma once
#include "PostEffect.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include <memory>

/**
* @file GlayScale.h
* @brief グレースケール用ポストエフェクトファイル
*/

namespace MNE
{

	class GlayScale :public PostEffect
	{
	private:
		MNE::PostEffect* original_ = nullptr;

		MNE::GPipeline* pipeline_ = nullptr;

	#pragma region ConstBuff

		MNE::CBuff::CBuffGlayScale* cGlayScaleMap_ = nullptr;
		MNE::ConstBuff activeGlay_;

	#pragma endregion

	public:
		/**
		* @fn Initialize(PostEffect*)
		* 初期化用関数
		* @param original ブラーをかける前の画像
		*/
		void Initialize(MNE::PostEffect* original);
		/**
		* @fn DrawGlay()
		* 描画処理関数
		*/
		void DrawGlay();

		void SetGlayScale(bool active);
	};

}
