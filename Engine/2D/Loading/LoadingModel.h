#pragma once
#include "ICamera.h"
#include "Object3D.h"
#include "ILoadingObj.h"
#include "IModel.h"

#include <memory>

/**
* @file LoadingModel.h
* @brief ILoadingObjを継承したモデルのローディング表示用処理をまとめたファイル
*/

class LoadingModel :public ILoadingObj
{
private:
	std::unique_ptr<ICamera> camera_;
	std::unique_ptr<Object3D> loadObj_;
	std::unique_ptr<IModel> loadModel_;

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
};

