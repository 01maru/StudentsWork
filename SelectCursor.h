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
	Sprite cursor_;
	FrameCounter counter_;
	Vector2D minSize_ = { 298, 82 };
	Vector2D maxSize_ = { 298, 82 };
	//	選択中のモノのサイズ
	Vector2D selectObjSize_;

	int32_t easePawNum_ = 2;

private:
	void AnimationUpdate();

public:
#pragma region Setter

	/**
	* @fn SetCursorPosition(const Vector2D&, bool)
	* カーソルの表示位置変更用関数
	* @param pos 選択位置の中心座標
	* @param playMoveSound 移動音再生するか
	*/
	void SetCursorPosition(const Vector2D& pos, bool playMoveSound = true);
	/**
	* @fn SetMinSize(const Vector2D&)
	* カーソル拡縮アニメーションの最小サイズ変更用関数
	* @param size カーソルの最小サイズ
	*/
	void SetMinSize(const Vector2D& size);
	/**
	* @fn SetMaxSize(const Vector2D&)
	* カーソル拡縮アニメーションの最大サイズ変更用関数
	* @param size カーソルの最大サイズ
	*/
	void SetMaxSize(const Vector2D& size);
	///**
	//* @fn SetSize(const Vector2D&)
	//* 選択中のモノのサイズ変更用関数
	//* @param size 選択中のモノのサイズ
	//*/
	//void SetSize(const Vector2D& size);
	/**
	* @fn SetIsActive(bool)
	* isActive_変更用関数
	* @param isActive isActive変更後の値
	*/
	void SetIsActive(bool isActive);

#pragma endregion
};

