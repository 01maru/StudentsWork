#include "UISprite.h"

void MNE::UISprite::MatUpdate()
{
	for (auto sprite : sprites_)
	{
		sprite.second.Update();
	}
}

void MNE::UISprite::Draw()
{
	for (auto sprite : sprites_)
	{
		sprite.second.Draw();
	}
}

//-----------------------------------------------------------------------------
// Add & Delete
//-----------------------------------------------------------------------------

void MNE::UISprite::AddSprite(const std::string& name, MNE::Sprite& sprite)
{
	sprites_.emplace(name, sprite);
}

void MNE::UISprite::DeleteSprite(const std::string& spriteName)
{
	if (sprites_.count(spriteName) == 0) return;

	sprites_.erase(spriteName);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

std::unordered_map<std::string, MNE::Sprite>& MNE::UISprite::GetSprites()
{
	return sprites_;
}
