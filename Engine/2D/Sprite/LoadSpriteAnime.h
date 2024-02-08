#pragma once
#include "LoadingSprite.h"
#include "FrameCounter.h"
#include "Vector2D.h"
#include <vector>

/**
* @file LoadSpriteAnime.h
* @brief ロード中を表す表示を管理するファイル
*/

class LoadSpriteAnime
{
public:
	/**
	* @fn Initialize(const Vector2D&, int32_t fadeTime)
	* 初期化関数
	* @param centerPos 表示する中心位置
	* @param fadeTime フェードインアウトの時間設定
	*/
	void Initialize(const MyMath::Vector2D& centerPos, int32_t fadeTime);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

private:
	//	アニメーション用タイマー
	MNE::FrameCounter counter_;
	//	スプライト配列
	std::vector<MNE::LoadingSprite> sprites_;
	//	スプライト1枚の正方形の1辺の長さ
	float spriteSize_ = 30.0f;

	std::vector<int32_t> index_ = { 0,1,3,6,8,9 };
	const int32_t MAX_ROW_NUM = 5;
	const int32_t TIME_BETWEEN_ANIME = 5;
	const int32_t WAIT_TIME = 20;

private:
	/**
	* @fn ActiveUpdate()
	* スプライト一つ一つのアニメーションを開始させるための更新処理関数
	*/
	void ActiveUpdate();

public:
#pragma region Setter

	/**
	* @fn Reset()
	* アニメーションリセット用関数
	*/
	void Reset();
	/**
	* @fn SetIsLoading(bool)
	* ロード中かどうかのセット用関数
	* @param loading ロード中かどうか
	*/
	void SetIsLoading(bool loading);

#pragma endregion
};

