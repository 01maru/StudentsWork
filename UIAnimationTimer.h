#pragma once
#include "UIComponent.h"
#include "FrameCounter.h"

class UIAnimationTimer :public UIComponent, public FrameCounter
{
private:
	int32_t startCount_ = 0;

public:
	~UIAnimationTimer() {};

	void Initialize() override;
	void Update() override;

	void SetStartCount(int32_t startCount) { startCount_ = startCount; }
	int32_t GetStartCount() { return startCount_; }
};

