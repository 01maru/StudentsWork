#pragma once
#include "EmitterComponent.h"

namespace MNE
{

	class EmitterSpdComp :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;
	};

}
