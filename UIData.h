#pragma once
#include "Sprite.h"
#include <map>

class UIData
{
private:
	uint16_t drawTag_ = 0;
	std::map<std::string, Sprite, std::less<>> sprites_;

public:
	void Update();
	void Draw();

	uint16_t GetTags() { return drawTag_; }

	void SetTag(uint16_t tag) { drawTag_ = tag; }
	void AddTag(uint16_t tag) { drawTag_ |= tag; }
	void SubTag(uint16_t tag) { drawTag_ ^= tag; }
	void LoadSprites(const std::string& filename);
	void SetSprites(const std::map<std::string, Sprite, std::less<>>& sprites) { sprites_ = sprites; }
};

