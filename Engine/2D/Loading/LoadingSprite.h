#pragma once
#include "ILoadingObj.h"
#include "Sprite.h"
#include <memory>

/**
* @file LoadingSprite.h
* @brief ILoadingObjを継承したスプライトのローディング表示用処理をまとめたファイル
*/

class LoadingSprite :public ILoadingObj
{
private:
	Texture* loadTex_;
	std::unique_ptr<Sprite> loadSprite_;
	
public:
	LoadingSprite() {}
	~LoadingSprite() override {}

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
