#include "EmitterConeType.h"
#include "MyMath.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

#include "Quaternion.h"

using namespace Easing;
using namespace MyMath;

//void MNE::EmitterConeType::Update(MNE::Particle* particle)
//{
//	float angleXZ = GetRand(0.0f, PIx2);
//	float lenRate = GetRand(0.0f, 1.0f);
//	
//	MyMath::Vector3D dirXZ = MyMath::Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));
//
//	float angle = ConvertToRad(angle_ - 90.0f);
//
//	dirXZ.Normalize();
//	MyMath::Vector3D up(0, 1, 0);
//	MyMath::Vector3D v = up.cross(dirXZ);
//
//	Quaternion q = SetQuaternion(v, angle);
//	Vector3D dir = RotateVector(dirXZ, q);
//	dir.Normalize();
//
//	coneDir_.Normalize();
//	Quaternion rotQ = DirectionToDirection(up, coneDir_);
//
//	dir = Vec3Transform(dir, rotQ.GetRotMatrix());
//	dirXZ = Vec3Transform(dirXZ, rotQ.GetRotMatrix());
//	pos_ = parent_->GetPosition() + radius_ * lenRate * dirXZ;
//
//	particle->SetDir(dir);
//
//	if (parent_->GetIsObj())
//	{
//		MNE::ObjectParticle* obj = particle->GetComponent<MNE::ObjectParticle>();
//
//		obj->SetPosition(pos_);
//	}
//	else
//	{
//		MNE::SpriteParticle* sprite = particle->GetComponent<MNE::SpriteParticle>();
//
//		sprite->SetPosition(pos_);
//	}
//}

std::vector<std::unique_ptr<MNE::Particle>> MNE::EmitterConeType::Update()
{
	std::vector<std::unique_ptr<MNE::Particle>> particles;

	float angle = ConvertToRad(angle_ - 90.0f);

	for (int32_t i = 0; i < particleNum_; i++)
	{
		particles.emplace_back(std::make_unique<Particle>());
		Particle* particle = particles.back().get();

		float angleXZ = GetRand(0.0f, PIx2);
		float lenRate = GetRand(0.0f, 1.0f);

		MyMath::Vector3D dirXZ = MyMath::Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));

		dirXZ.Normalize();
		MyMath::Vector3D up(0, 1, 0);
		MyMath::Vector3D v = up.cross(dirXZ);

		Quaternion q = SetQuaternion(v, angle);
		Vector3D dir = RotateVector(dirXZ, q);
		dir.Normalize();

		dir_.Normalize();
		Quaternion rotQ = DirectionToDirection(up, dir_);

		dir = Vec3Transform(dir, rotQ.GetRotMatrix());
		dirXZ = Vec3Transform(dirXZ, rotQ.GetRotMatrix());
		Vector3D pos_ = parent_->GetPosition() + radius_ * lenRate * dirXZ;

		particle->SetDir(dir);

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
