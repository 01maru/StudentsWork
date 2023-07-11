#pragma once
#include "Sprite.h"
#include <map>

class UIData
{
public:
	enum TagENum
	{
		Tag1  = 0b1 << 0,
		Tag2  = 0b1 << 1,
		Tag3  = 0b1 << 2,
		Tag4  = 0b1 << 3,
		Tag5  = 0b1 << 4,
		Tag6  = 0b1 << 5,
		Tag7  = 0b1 << 6,
		Tag8  = 0b1 << 7,
		Tag9  = 0b1 << 8,
		Tag10 = 0b1 << 9,
		Tag11 = 0b1 << 10,
		Tag12 = 0b1 << 11,
		Tag13 = 0b1 << 12,
		Tag14 = 0b1 << 13,
		Tag15 = 0b1 << 14,
		Tag16 = 0b1 << 15,
	};
private:
	uint16_t drawTag_ = 0b1111111111111111;
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

