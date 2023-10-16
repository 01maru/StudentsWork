#pragma once
#include "Sprite.h"
#include "UIButtonManager.h"
#include <map>

/**
* @file UIData.h
* @brief UIEditor用のUIData構造体ファイル
*/

struct UIData
{
	std::map<std::string, uint16_t, std::less<>> tagName_;
	std::map<std::string, Sprite, std::less<>> sprites_;
	UIButtonManager buttonMan_;
};

