#pragma once
#include "UIData.h"
#include "OptionScene.h"

class PauseScreen
{
public:
	enum Mord
	{
		Continue,
		Option,
		BackTitle,
	};

private:
	uint16_t selectMord_;

	bool active_ = true;

	std::unique_ptr<UIData> ui_;
	std::unique_ptr<OptionScene> option_;

private:
	void PauseUpdate();

public:
	void Initialize();
	void Update();
	void Draw();

	//	Getter
	bool GetIsActive() { return active_; }
	uint16_t GetOptionMord() { return option_->GetMord(); }
	uint16_t GetMord() { return selectMord_; }
	bool GetOptionIsActive() { return option_->GetIsActive(); }

	//	Setter
	void SetIsActive(bool active);
	void SetMord(Mord mord) { selectMord_ = (uint16_t)mord; }
};

