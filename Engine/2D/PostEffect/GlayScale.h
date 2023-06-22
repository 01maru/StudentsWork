#pragma once
#include "PostEffect.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include <memory>

class GlayScale :public PostEffect
{
private:
	PostEffect* original_ = nullptr;

	GPipeline* pipeline_ = nullptr;

#pragma region ConstBuff

	CBuff::CBuffGlayScale* cGlayScaleMap_ = nullptr;
	ConstBuff activeGlay_;

#pragma endregion

public:
	void Initialize(PostEffect* original);
	void DrawGlay();

	void SetGlayScale(bool active);
};

