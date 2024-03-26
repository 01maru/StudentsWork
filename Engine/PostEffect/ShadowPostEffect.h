#pragma once
#include "IPostEffect.h"

namespace MNE
{

	class ShadowPostEffect :public IPostEffect
	{
	public:
		void Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		void Update() override;
	};

}

