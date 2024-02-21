#pragma once
#include "FrameCounter.h"
#include "Sprite.h"

/**
* @file SkillActiveEffect.h
* @brief スキルのクールタイム終了を知らせるエフェクトを管理するファイル
*/

class SkillActiveEffect
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();
	/**
	* @fn LoadResources()
	* リソース読み込み関数
	*/
	void LoadResources();
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
	//	タイマー
	int32_t startTime_ = 10;
	int32_t endTime_ = 5;
	MNE::FrameCounter timer_;

	//	スプライト
	MNE::Sprite sprite_;
	//	開始位置
	MyMath::Vector2D startPos_;
	//	終了時のスプライトの中心座標
	MyMath::Vector2D endPos_;
	//	画像サイズ
	MyMath::Vector2D texSize_;

	float maxSizeY_ = 504.0f;
	float endSizeY_ = 400.0f;

private:
	/**
	* @fn InitEndAnimation()
	* 終了アニメーション初期化関数
	*/
	void InitEndAnimation();
	/**
	* @fn StartAnimeUpdate()
	* 開始アニメーション更新処理関数
	*/
	void StartAnimeUpdate();
	/**
	* @fn EndAnimeUpdate()
	* 終了アニメーション更新処理関数
	*/
	void EndAnimeUpdate();

public:
#pragma region Setter

	/**
	* @fn Start()
	* アニメーション開始用関数
	*/
	void Start();
	/**
	* @fn SetStartPos(const MyMath::Vector2D&)
	* アニメーション開始位置設定用関数
	* @param pos 開始位置
	*/
	void SetStartPos(const MyMath::Vector2D& pos);

#pragma endregion
};

