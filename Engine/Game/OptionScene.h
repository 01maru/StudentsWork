#pragma once
#include "SliderSprite.h"
#include "UIData.h"

class OptionScene :public UIData
{
public:
	enum Mord
	{
		Sens,
		Sound,
		Close,
	};
private:
	bool active_ = false;

	uint16_t mord_;

	//	音量
	//	光度
public:
	void Initialize(const std::string& filename);
	void Update();
	void Draw();

	//	Getter
	bool GetIsActive() { return active_; }
	uint16_t GetMord() { return mord_; }

	//	Setter
	void SetIsActive(bool active) { active_ = active; }
};

