#pragma once
#include "LoadingSprite.h"
#include "FrameCounter.h"
#include "Vector2D.h"
#include <vector>

class LoadSpriteAnime
{
public:
	void Initialize(const Vector2D& centerPos, int32_t fadeTime);
	void Update();
	void Draw();

	void Reset();
private:
	std::vector<MNE::LoadingSprite> sprites_;
	FrameCounter counter_;
	float spriteSize_ = 30.0f;
	const int32_t MAX_SPRITE_NUM = 9;
	std::vector<int32_t> index_ = { 0,1,3,6,8,9 };
	int32_t rowNum_ = 0;
	const int32_t MAX_ROW_NUM = 5;
	const int32_t TIME = 5;
	const int32_t WAIT_TIME = 20;

private:
	void ActiveUpdate();
public:
	void SetIsLoading(bool loading);
};

