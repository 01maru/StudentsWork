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

	MNE::Sprite sprite_;
	MNE::Sprite text_;
	MNE::Sprite gauge_;

private:
	void SpriteUpdate();

public:
	bool GetIsActive();
#pragma region Setter

	void SetSprite(const MNE::Sprite& sprite);
	void SetTextSprite(const MNE::Sprite& sprite);
	void SetMaxTime(int32_t time);

#pragma endregion
};

