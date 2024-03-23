#pragma once
#include "IPostEffect.h"
#include <array>
#include <memory>

/**
* @file GaussBlur.h
* @brief ガウシアンブラー用ポストエフェクトファイル
*/

namespace MNE
{
	class GaussBlur;
	namespace CBuff
	{
		struct CBufferBlurWeight;
	}

	class GaussBlurPostEffect :public IPostEffect
	{
	public:
		void Draw(int32_t mode = 0) override;

	private:
		GaussBlur* parent_ = nullptr;

	public:
		void SetGaussBlur(GaussBlur* gaussBlur);
	};

	class GaussBlur
	{
	public:
		/**
		* @fn Initialize(float, PostEffect*, DXGI_FORMAT)
		* 初期化用関数
		* @param weight ブラーの強さ
		* @param original ブラーをかける前の画像
		* @param index ブラーかけた後の結果のフォーマット指定
		*/
		void Initialize(IPostEffect* original, DXGI_FORMAT format);

	private:
		GaussBlurPostEffect* blurX_ = nullptr;
		GaussBlurPostEffect* blurY_ = nullptr;

		Texture* tex;

	#pragma region ConstBuff

		CBuff::CBufferBlurWeight* mapWeight_ = nullptr;
		ConstBuff weight_;
		std::array<float, MyMath::WEIDHTS_NUM> weights_;

	#pragma endregion

	public:

	#pragma region Getter

		/**
		* @fn GetTexture(int32_t)
		* ブラーかけた後の結果を返す関数
		* @param index 画像のインデックス
		* @return ブラーかけた後の結果
		*/
		Texture* GetTexture(int32_t /*index*/) { return tex; }

	#pragma endregion

	#pragma region Setter

		void SetWeightGraphicsRootCBuffView(int32_t rootparaIdx);
		void SetWeight(float weight);
		void SetPipeline(GPipeline* blurXPipeline, GPipeline* blurYPipeline);
		void SetClearColor(const MyMath::Vector4D& color);

	#pragma endregion
	};

}
