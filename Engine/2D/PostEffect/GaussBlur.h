#pragma once
#include "GaussBlurX.h"
#include "ConstBuff.h"

class PostEffect;

class GaussBlur
{
private:
	PostEffect* origin_;

	GaussBlurX blurX_;
#pragma region ConstBuff

	ConstBuff weight_;

#pragma endregion

	std::vector<float> weights_;

public:
	void Initialize(float weight);
	void Draw();

	//	Setter
	void SetOrigin(PostEffect* origin) { origin_ = origin; }
};

