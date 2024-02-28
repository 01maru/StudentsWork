#include "EmitterSphereType.h"
#include "MyMath.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

#include "Quaternion.h"

using namespace Easing;
using namespace MyMath;

std::vector<std::unique_ptr<MNE::Particle>> MNE::EmitterSphereType::Update()
{
	std::vector<std::unique_ptr<MNE::Particle>> particles;

	for (int32_t i = 0; i < particleNum_; i++)
	{
		particles.emplace_back(std::make_unique<Particle>());
		Particle* particle = particles.back().get();

		float angleXZ = GetRand(0.0f, PIx2);
		float lenRate = GetRand(radiusThickness_, 1.0f);

		MyMath::Vector3D dirXZ = MyMath::Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));

		float angle = GetRand(0.0f, PIx2);

		dirXZ.Normalize();
		MyMath::Vector3D v = MyMath::Vector3D(0, 1, 0).cross(dirXZ);

		Quaternion q = SetQuaternion(v, angle);
		Vector3D dir = RotateVector(dirXZ, q);
		dir.Normalize();

		Vector3D pos_ = parent_->GetPosition() + radius_ * dir * lenRate;

		particle->SetDir(dir);

		if (parent_->GetIsObj())
		{
			MNE::ObjectParticle* obj = particle->AddComponent<MNE::ObjectParticle>();

			obj->SetPosition(pos_);
		}
		else
		{
			MNE::SpriteParticle* sprite = particle->AddComponent<MNE::SpriteParticle>();

			sprite->SetPosition(pos_);
		}
	}

	return particles;
}

void MNE::EmitterSphereType::SetRadius(float radius)
{
	radius_ = radius;
}

void MNE::EmitterSphereType::SetRadiusThickness(float thickness)
{
	radiusThickness_ = mClamp(0.0f, 1.0f, thickness);
}
