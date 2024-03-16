#pragma once
#include "IPostEffect.h"

namespace MNE
{

	class ShadowPostEffect :public IPostEffect
	{
	public:
		void Update() override;
	};

}

