#pragma once
#include "EmitterComponent.h"
#include "ParticleValue.h"

namespace MNE
{

	class EmitterFadeAnimation :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;

	private:
		float endValue_ = 0.0f;
		float startValue_ = 1.0f;

	public:
		void SetEndValue(float v);
		void SetStartValue(float v);
	};

}
