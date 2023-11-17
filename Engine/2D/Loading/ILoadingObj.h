#pragma once
#include "FrameCounter.h"
#include <cstdint>

/**
* @file ILoadingObj.h
* @brief 継承するローディング表示用処理をまとめたファイル
*/

class ILoadingObj
{
public:
	virtual ~ILoadingObj() = default;
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	virtual void Initialize() = 0;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw() = 0;

protected:
	//	ロード中か
	bool loading_ = false;
	bool drawObj_ = false;
	//	フェードインアウト用
	FrameCounter counter_;

public:
#pragma region Setter

	void SetIsLoading(bool loading);
	void SetFadeAnimeTime(int32_t time);

#pragma endregion
};

