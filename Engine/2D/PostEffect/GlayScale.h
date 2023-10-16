#pragma once
#include "PostEffect.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include <memory>

/**
* @file GlayScale.h
* @brief グレースケール用ポストエフェクトファイル
*/

class GlayScale :public PostEffect
{
private:
	PostEffect* original_ = nullptr;

	GPipeline* pipeline_ = nullptr;

#pragma region ConstBuff

	CBuff::CBuffGlayScale* cGlayScaleMap_ = nullptr;
	ConstBuff activeGlay_;

#pragma endregion

public:
	/**
	* @fn Initialize(PostEffect*)
	* 初期化用関数
	* @param original ブラーをかける前の画像
	*/
	void Initialize(PostEffect* original);
	/**
	* @fn DrawGlay()
	* 描画処理関数
	*/
	void DrawGlay();

	void SetGlayScale(bool active);
};

