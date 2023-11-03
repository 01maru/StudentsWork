#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

class SelectCursor
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();

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
	Sprite cursor_;
	FrameCounter counter_;
	Vector2D minSize_ = { 298, 82 };
	Vector2D maxSize_ = { 298, 82 };
public:
	void SetCursorPosition(const Vector2D& pos);
	void SetMinSize(const Vector2D& size);
	void SetMaxSize(const Vector2D& size);
};

