#pragma once
#include "EmitterComponent.h"
#include "ParticleValue.h"

namespace MNE
{

	class EmitterRotation :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;

	private:
		std::unique_ptr<ParticleValue> value_;
		std::unique_ptr<ParticleValue> add_;

	public:
		void SetValue(std::unique_ptr<ParticleValue>& v);
		void SetAddValue(std::unique_ptr<ParticleValue>& v);
	};

}
