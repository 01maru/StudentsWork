#pragma once
#include "EmitterType.h"

namespace MNE
{

	class EmitterCircleType :public EmitterType
	{
	public:
		std::vector<std::unique_ptr<MNE::Particle>> Update() override;

	private:
		//	円の向き
		MyMath::Vector3D particleDir_ = { 0.0f,1.0f,0.0f };
		//	半径
		float radius_ = 0.1f;

	public:
		void SetParticleDir(const MyMath::Vector3D& dir);
	};

}

