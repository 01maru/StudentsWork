#include "EmitterCircumType.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Quaternion.h"

using namespace MyMath;

std::vector<std::unique_ptr<MNE::Particle>> MNE::EmitterCircumType::Update()
{
	std::vector<std::unique_ptr<MNE::Particle>> particles;

	Vector3D axisY(0, 1, 0);
	Vector3D normal;
	Quaternion rotQ = DirectionToDirection(axisY, dir_);

	float angle = PIx2 / static_cast<float>(particleNum_);
	Quaternion rotY = SetQuaternion(axisY, angle);

	MyMath::Vector3D dirXZ(1.0f, 0.0f, 0.0f);

	for (int32_t i = 0; i < particleNum_; i++)
	{
		particles.emplace_back(std::make_unique<Particle>());
		Particle* particle = particles.back().get();

		dirXZ = RotateVector(dirXZ, rotY);

		Vector3D dir = Vec3Transform(dirXZ, rotQ.GetRotMatrix());

		particle->SetDir(dir);

		Vector3D pos_ = parent_->GetPosition() + radius_ * dir;

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
