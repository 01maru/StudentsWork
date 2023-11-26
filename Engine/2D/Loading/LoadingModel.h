#pragma once
#include "Object3DShilhouette.h"
#include "ILoadingObj.h"

/**
* @file LoadingModel.h
* @brief ILoadingObjを継承したモデルのローディング表示用処理をまとめたファイル
*/

class LoadingModel :public ILoadingObj, public Object3DShilhouette
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

private:
	int32_t easePaw_ = 4;
	float rotSpd_ = 0.05f;
};

