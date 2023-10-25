#pragma once
#include "Sprite.h"
#include "UIComponent.h"
#include <unordered_map>

/**
* @file UISprite.h
* @brief UIObjectでspriteの描画を管理する機能をまとめたファイル
*/

class UISprite :public UIComponent
{
public:
	/**
	* @fn MatUpdate()
	* 座標更新処理関数
	*/
	void MatUpdate() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

private:
	//	描画するスプライトのマップ
	std::unordered_map<std::string, Sprite> sprites_;

public:
	/**
	* @fn AddSprite(const std::string&, Sprite&)
	* sprites_に新しいスプライトを追加するための関数
	* @param name 新しいスプライトの名前
	* @param sprite 新しいスプライトの情報
	*/
	void AddSprite(const std::string& name, Sprite& sprite);
	/**
	* @fn DeleteSprite(const std::string&)
	* sprites_からスプライトを削除するための関数
	* @param spriteName 削除するスプライトの名前
	*/
	void DeleteSprite(const std::string& spriteName);

#pragma region Getter

	/**
	* @fn GetSprites()
	* 描画するスプライトのマップ型変数のGetter関数
	* @return 描画するスプライトのマップ型変数
	*/
	std::unordered_map<std::string, Sprite>& GetSprites();

#pragma endregion
};

