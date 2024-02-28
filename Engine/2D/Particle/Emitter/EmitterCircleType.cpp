#include "EmitterCircleType.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Quaternion.h"

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

std::vector<std::unique_ptr<MNE::Particle>> MNE::EmitterCircleType::Update()
{
	std::vector<std::unique_ptr<MNE::Particle>> particles;

	Vector3D axisY(0, 1, 0);
	Vector3D normal;
	Quaternion rotQ = DirectionToDirection(axisY, dir_);

	for (int32_t i = 0; i < particleNum_; i++)
	{
		particles.emplace_back(std::make_unique<Particle>());
		Particle* particle = particles.back().get();

		float angleXZ = GetRand(0.0f, PIx2);
		float lenRate = GetRand(0.0f, 1.0f);

		MyMath::Vector3D dirXZ = MyMath::Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));
		dirXZ.Normalize();

		dirXZ = Vec3Transform(dirXZ, rotQ.GetRotMatrix());
		
		particle->SetDir(particleDir_);
		
		Vector3D pos_ = parent_->GetPosition() + radius_ * lenRate * dirXZ;

		if (parent_->GetIsObj()) {
			ObjectParticle* object = particle->AddComponent<ObjectParticle>();
			object->SetPosition(pos_);
		}
		else {
			SpriteParticle* sprite = particle->AddComponent<SpriteParticle>();
			sprite->SetPosition(pos_);
		}
	}

	return particles;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::EmitterCircleType::SetParticleDir(const MyMath::Vector3D& dir)
{
	particleDir_ = dir;
}
