#include "EmitterSphereType.h"
#include "MyMath.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

#include "Quaternion.h"

using namespace Easing;
using namespace MyMath;

void MNE::EmitterSphereType::Update(MNE::Particle* particle)
{
	float angleXZ = GetRand(0.0f, PIx2);
	float lenRate = GetRand(radiusThickness_, 1.0f);

	MyMath::Vector3D dirXZ = MyMath::Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));

	float angle = (GetRand(0.0f, PIx2));

	dirXZ.Normalize();
	MyMath::Vector3D v = MyMath::Vector3D(0, 1, 0).cross(dirXZ);

	Quaternion q = SetQuaternion(v, angle);
	dir_ = RotateVector(dirXZ, q);
	dir_.Normalize();

	pos_ = parent_->GetPosition() + radius_ * dir_ * lenRate;

	if (parent_->GetIsObj())
	{
		MNE::ObjectParticle* obj = particle->GetComponent<MNE::ObjectParticle>();

		obj->SetPosition(pos_);
	}
	else
	{
		MNE::SpriteParticle* sprite = particle->GetComponent<MNE::SpriteParticle>();

		sprite->SetPosition(pos_);
	}
}

void MNE::EmitterSphereType::SetRadius(float radius)
{
	radius_ = radius;
}

void MNE::EmitterSphereType::SetRadiusThickness(float thickness)
{
	radiusThickness_ = mClamp(0.0f, 1.0f, thickness);
}
