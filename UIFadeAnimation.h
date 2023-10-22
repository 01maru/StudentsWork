#pragma once
#include "UIComponent.h"
#include "Vector2D.h"

class UIAnimationTimer;
class UISprite;

class UIFadeAnimation :public UIComponent
{
private:
	UISprite* sprite_ = nullptr;
	UIAnimationTimer* timer_ = nullptr;
	float start_;
	float end_;
	
public:
	void Initialize() override;
	void Update() override;

	void SetStart(float start) { start_ = start; }
	void SetEnd(float end) { end_ = end; }

	float GetStart() { return start_; }
	float GetEnd() { return start_; }
};

