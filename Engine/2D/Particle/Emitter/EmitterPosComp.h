#pragma once
#include "EmitterComponent.h"
#include "ParticleValue.h"

namespace MNE
{

	class EmitterPosComp :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;

	private:
		std::unique_ptr<ParticleValue> add_;

	public:
		void SetAddValue(std::unique_ptr<ParticleValue>& v);
	};

}
