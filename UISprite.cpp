#include "UISprite.h"

void UISprite::MatUpdate()
{
	for (auto sprite : sprites_)
	{
		sprite.second.Update();
	}
}

void UISprite::Draw()
{
	for (auto sprite : sprites_)
	{
		sprite.second.Draw();
	}
}

void UISprite::DeleteSprite(const std::string& spriteName)
{
	if (sprites_.count(spriteName) == 0) return;

	sprites_.erase(spriteName);
}
