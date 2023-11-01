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

//-----------------------------------------------------------------------------
// Add & Delete
//-----------------------------------------------------------------------------

void UISprite::AddSprite(const std::string& name, Sprite& sprite)
{
	sprites_.emplace(name, sprite);
}

void UISprite::DeleteSprite(const std::string& spriteName)
{
	if (sprites_.count(spriteName) == 0) return;

	sprites_.erase(spriteName);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

std::unordered_map<std::string, Sprite>& UISprite::GetSprites()
{
	return sprites_;
}
