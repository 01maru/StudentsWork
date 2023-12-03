#pragma once
#include "FrameCounter.h"
#include "Sprite.h"

class CoolTime
{
public:
	void Initialize();

	virtual void Update();

	void Draw();

	void StartCount();
protected:
	bool isActive_ = false;
	FrameCounter coolTimer_;

	Sprite sprite_;
	Sprite text_;
	Sprite gauge_;

private:
	void SpriteUpdate();

public:
	bool GetIsActive();
#pragma region Setter

	void SetSprite(const Sprite& sprite);
	void SetTextSprite(const Sprite& sprite);
	void SetMaxTime(int32_t time);

#pragma endregion
};

