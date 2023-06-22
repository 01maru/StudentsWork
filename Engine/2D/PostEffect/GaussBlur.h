#pragma once
#include "PostEffect.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include <memory>

class GaussBlur
{
private:
	std::unique_ptr<PostEffect> blurX_;
	std::unique_ptr<PostEffect> blurY_;

	GPipeline* pipeline[2];

	PostEffect* original_ = nullptr;

#pragma region ConstBuff

	ConstBuff weight_;
	std::vector<float> weights_;

#pragma endregion

public:
	void Initialize(float weight, PostEffect* original, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	void Draw();

	//	Getter
	Texture* GetTexture(int32_t index) { return blurY_->GetTexture(index); }

	//	Setter
	void SetPipeline(GPipeline* blurXPipeline, GPipeline* blurYPipeline);
	void SetClearColor(const Vector4D& color);
};

