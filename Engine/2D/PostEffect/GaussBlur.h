#pragma once
#include "PostEffect.h"

class GaussBlur
{
private:
	PostEffect* origin_ = nullptr;
	PostEffect postEffect_[2];

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

