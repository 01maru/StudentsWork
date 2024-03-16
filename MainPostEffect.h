#pragma once
#include "IPostEffect.h"

namespace MNE
{

	class MainPostEffect :public IPostEffect
	{
	public:
		enum DrawMord
		{
			Normal,
			Luminance,

		};

	public:
		void Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		void Update() override;
		void Draw(int32_t mode = 0) override;

	private:
		GPipeline* luminancePipe_ = nullptr;

	private:
		void DrawLuminance();
		void DrawNormal();
	};

}
