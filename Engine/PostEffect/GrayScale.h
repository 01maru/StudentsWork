#pragma once
#include "IPostEffect.h"

/**
* @file GrayScale.h
* @brief グレースケール用ポストエフェクトファイル
*/

namespace MNE
{
	namespace CBuff
	{
		struct CBuffGrayScale;
	}

	class GrayScale: public IPostEffect
	{
	public:
		void Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		void Draw(int32_t mode = 0) override;

	private:
#pragma region ConstBuff

		CBuff::CBuffGrayScale* cGrayScaleMap_ = nullptr;
		ConstBuff activeGray_;

#pragma endregion

	public:
		bool GetActiveGrayScale();
		void SetActiveGrayScale(bool active);
	};
}

