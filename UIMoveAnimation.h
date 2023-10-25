#pragma once
#include "UIComponent.h"
#include "Vector2D.h"

/**
* @file UIMoveAnimation.h
* @brief UI移動アニメーション用のファイル
*/

#pragma region 前置宣言

class UIAnimationTimer;
class UISprite;

#pragma endregion

class UIMoveAnimation :public UIComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	//	動かすスプライト
	UISprite* sprite_ = nullptr;
	//	全体のtimer
	UIAnimationTimer* timer_ = nullptr;
	//	初期位置
	Vector2D startPos;
	//	終了位置
	Vector2D endPos;

public:
#pragma region Getter

	/**
	* @fn GetStartPos()
	* 初期位置のGetter関数
	* @return 初期位置
	*/
	Vector2D& GetStartPos();
	/**
	* @fn GetStartPos()
	* 終了位置のGetter関数
	* @return 終了位置
	*/
	Vector2D& GetEndPos();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetStartPos(const Vector2D&)
	* 初期位置を変更するための関数
	* @param pos 初期位置の変更値
	*/
	void SetStartPos(const Vector2D& pos);
	/**
	* @fn SetEndPos(const Vector2D&)
	* 終了位置を変更するための関数
	* @param pos 終了位置の変更値
	*/
	void SetEndPos(const Vector2D& pos);

#pragma endregion
};

