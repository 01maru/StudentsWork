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
		* @fn Initialize(float, PostEffect*)
		* 初期化用関数
		* @param original ブラーをかける元のポストエフェクト
		*/
		void Initialize(IPostEffect* original);

	private:
		IPostEffect* blurX_ = nullptr;
		IPostEffect* blurY_ = nullptr;

		Texture* tex;

	#pragma region ConstBuff

		CBuff::CBufferBlurWeight* mapWeight_ = nullptr;
		ConstBuff weight_;
		std::array<float, MyMath::WEIDHTS_NUM> weights_;

	#pragma endregion

	private:
		void AddPipeline(DXGI_FORMAT format);

	public:

	#pragma region Getter

		/**
		* @fn GetBlurredTexture()
		* ブラーかけた後の結果を返す関数
		*/
		Texture* GetBlurredTexture();

	#pragma endregion

	#pragma region Setter

		void SetWeightGraphicsRootCBuffView(int32_t rootparaIdx);
		void SetWeight(float weight);
		void SetPipeline(GPipeline* blurXPipeline, GPipeline* blurYPipeline);
		void SetClearColor(const MyMath::Vector4D& color);

	#pragma endregion
	};

}
