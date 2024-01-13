#pragma once
#include "Vector3D.h"
#include "Particle.h"

namespace MNE
{
	
	class ParticleEmitter;

	class EmitterType
	{
	public:
		~EmitterType() = default;
		virtual void Update(MNE::Particle* particle);

	protected:
		MNE::ParticleEmitter* parent_;
		Vector3D dir_;
		Vector3D pos_;

	public:
		void SetParent(MNE::ParticleEmitter* parent);
		void SetDir(const Vector3D& dir) { dir_ = dir; }
		Vector3D GetDir() { return dir_; }
		Vector3D GetPosition() { return pos_; }
	};

}
