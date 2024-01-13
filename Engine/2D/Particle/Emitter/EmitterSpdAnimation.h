#pragma once
#include "EmitterComponent.h"

namespace MNE
{

	class EmitterSpdAnimation :public MNE::EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;
	
	private:
		float spd_ = 0.05f;
	public:
		void SetSpd(float spd) { spd_ = spd; }
	};

}
