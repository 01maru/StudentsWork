#pragma once
#include "Vector3D.h"
#include "Particle.h"
#include <vector>

namespace MNE
{
	
	class ParticleEmitter;

	class EmitterType
	{
	public:
		~EmitterType() = default;
		virtual std::vector<std::unique_ptr<MNE::Particle>> Update();

	protected:
		MNE::ParticleEmitter* parent_;
		MyMath::Vector3D dir_;
		//MyMath::Vector3D pos_;
		//	一度に生成するパーティクル数
		int32_t particleNum_ = 1;

	public:
		void SetParent(MNE::ParticleEmitter* parent);
		void SetDir(const MyMath::Vector3D& dir) { dir_ = dir; }
		MyMath::Vector3D GetDir() { return dir_; }
		//MyMath::Vector3D GetPosition() { return pos_; }

		void SetParticleNum(int32_t num);
	};

}
