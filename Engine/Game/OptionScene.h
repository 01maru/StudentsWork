#pragma once
#include "SliderSprite.h"

class OptionScene
{
private:
	//	感度
	float sensitivity_ = 1.0f;
	//	音量
	//	光度
public:
	void Initialize();
	void Update();
	void Draw();
};

