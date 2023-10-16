#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

/**
* @file SplashSprite.h
* @brief スプラッシュスクリーン用のカウント機能付きスプライトファイル
*/

class SplashSprite :public Sprite
{
private:
	std::unique_ptr<FrameCounter> count_;

public:
	/**
	* @fn Initialize(Texture*)
	* 初期化用関数
	* @param texture 表示する画像(何も設定しなかったら白色画像になる)
	*/
	void Initialize(Texture* texture = nullptr);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw(GPipeline*)
	* 描画処理関数
	*/
	void Draw();

	void ActiveCounter() { if (!count_->GetIsActive()) count_->StartCount(); }
};

