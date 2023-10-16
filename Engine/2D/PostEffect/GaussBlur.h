#pragma once
#include "PostEffect.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include <memory>

/**
* @file GaussBlur.h
* @brief ガウシアンブラー用ポストエフェクトファイル
*/

class GaussBlur
{
private:
	std::unique_ptr<PostEffect> blurX_;
	std::unique_ptr<PostEffect> blurY_;

	GPipeline* pipeline[2];

	PostEffect* original_ = nullptr;

#pragma region ConstBuff

	ConstBuff weight_;
	std::vector<float> weights_;

#pragma endregion

public:
	/**
	* @fn Initialize(float, PostEffect*, DXGI_FORMAT)
	* 初期化用関数
	* @param weight ブラーの強さ
	* @param original ブラーをかける前の画像
	* @param index ブラーかけた後の結果のフォーマット指定
	*/
	void Initialize(float weight, PostEffect* original, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

#pragma region Getter

	/**
	* @fn GetTexture(int32_t)
	* ブラーかけた後の結果を返す関数
	* @param index 画像のインデックス
	* @return ブラーかけた後の結果
	*/
	Texture* GetTexture(int32_t index) { return blurY_->GetTexture(index); }

#pragma endregion

#pragma region Setter

	void SetPipeline(GPipeline* blurXPipeline, GPipeline* blurYPipeline);
	void SetClearColor(const Vector4D& color);

#pragma endregion
};

