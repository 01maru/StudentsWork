#pragma once
#include "SliderSprite.h"
#include "UIData.h"

class OptionScene :public UIData
{
private:
	bool active_ = false;

	//	音量
	//	光度
public:
	void Initialize(const std::string& filename);
	void Update();
	void Draw();

	//	Getter
	bool GetIsActive() { return active_; }

	//	Setter
	void SetIsActive(bool active) { active_ = active; }
};

