#pragma once
#include "EmitterType.h"

namespace MNE
{

	 class EmitterSphereType :public EmitterType
	{
	public:
		void Update(MNE::Particle* particle) override;

	private:
		float radiusThickness_ = 0.0f;
		float radius_ = 1.0f;

	public:
		void SetRadius(float radius);
		void SetRadiusThickness(float thickness);
	};

}