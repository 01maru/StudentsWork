#pragma once
#include "EmitterType.h"

namespace MNE
{

	class EmitterSphereType :public EmitterType
	{
	public:
		std::vector<std::unique_ptr<MNE::Particle>> Update() override;

	private:
		float radiusThickness_ = 0.0f;
		float radius_ = 1.0f;

	public:
		void SetRadius(float radius);
		void SetRadiusThickness(float thickness);
	};

}