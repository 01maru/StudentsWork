#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

/**
* @file SelectCursor.h
* @brief 選択中のカーソルを表示やアニメーションさせるためのファイル
*/

class SelectCursor
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn LoadResources()
	* リソース読み込み用関数
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
	bool isActive_ = true;
	MNE::Sprite cursor_;
	const int32_t ANIME_TIME = 40;
	MNE::FrameCounter counter_;
	MyMath::Vector2D minSize_;
	MyMath::Vector2D maxSize_;
	//	選択中のモノのサイズ
	MyMath::Vector2D gapSize_ = MyMath::Vector2D(10.0f, 10.0f);

	int32_t easePawNum_ = 2;

private:
	/**
	* @fn AnimationUpdate()
	* 拡縮アニメーション用更新処理関数
	*/
	void AnimationUpdate();

public:
#pragma region Setter

	/**
	* @fn SetCursorPosition(const Vector2D&, bool)
	* カーソルの表示位置変更用関数
	* @param pos 選択位置の中心座標
	* @param playMoveSound 移動音再生するか
	*/
	void SetCursorPosition(const MyMath::Vector2D& pos, bool playMoveSound = true);
	/**
	* @fn SetSize(const Vector2D&)
	* 選択中のモノのサイズ変更用関数
	* @param size 選択中のモノのサイズ
	*/
	void SetButtonSize(const MyMath::Vector2D& size);
	/**
	* @fn SetIsActive(bool)
	* isActive_変更用関数
	* @param isActive isActive変更後の値
	*/
	void SetIsActive(bool isActive);

#pragma endregion
};

