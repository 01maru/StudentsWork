#pragma once
#include <cstdint>

/**
* @file ILoadingObj.h
* @brief 継承するローディング表示用処理をまとめたファイル
*/

class ILoadingObj
{
protected:
	bool fadeIn_ = false;
	//	ロード中か
	bool loading_ = false;
	//	フェードインアウト用
	static const int sEASE_MAX_COUNT = 30;
	int32_t easeCount_ = 0;

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
	virtual void Update() = 0;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw() = 0;

	void SetIsLoading(bool loading);
};

