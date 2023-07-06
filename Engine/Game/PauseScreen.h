﻿#pragma once
#include <map>
#include "Sprite.h"

class PauseScreen
{
private:
	bool active_ = true;
	bool isEnd_ = false;

	std::string filename_;
	std::map<std::string, Sprite, std::less<>> sprites_;

public:
	void Initialize();
	void Update();
	void Draw();

	//	Getter
	bool GetIsActive() { return active_; }
	bool GetIsEnd() { return isEnd_; }

	//	Setter
	void SetIsActive(bool active);
	void SetFileName(const std::string& filename);
};

