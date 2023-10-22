#pragma once
#include "Sprite.h"
#include "UIButtonManager.h"
#include "FrameCounter.h"
#include <map>
#include "UIObject.h"

/**
* @file UIData.h
* @brief UIEditor用のUIData構造体ファイル
*/

class UIData
{
protected:
	std::unique_ptr<FrameCounter> count_;
	std::map<std::string, uint16_t, std::less<>> tagName_;
	//std::map<std::string, Sprite, std::less<>> sprites_;
	std::map<std::string, std::unique_ptr<UIObject>, std::less<>> obj_;
	UIButtonManager buttonMan_;

public:
	void LoadData(const std::string& filename);
	void Finalize();

	void Update();
	void Draw();
};

