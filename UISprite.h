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
private:
	std::unordered_map<std::string, Sprite> sprites_;

public:
	~UISprite() {};
	void MatUpdate() override;
	void Draw() override;

	void AddSprite(const std::string& name, Sprite& sprite) { sprites_.emplace(name, sprite); }
	void DeleteSprite(const std::string& spriteName);

	std::unordered_map<std::string, Sprite>& GetSprites() { return sprites_; }
};

