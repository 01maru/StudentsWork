#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

class SplashSprite :public Sprite
{
private:
	std::unique_ptr<FrameCounter> count_;

public:
	void Initialize(Texture* texture = nullptr);
	void Update();
	void Draw();

	void ActiveCounter() { if (!count_->GetIsActive()) count_->StartCount(); }
};

