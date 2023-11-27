#include "EmitterSphereType.h"
#include "MyMath.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

#include "Quaternion.h"

using namespace Easing;
using namespace MyMath;

void EmitterSphereType::Update(Particle* particle)
{
	float angleXZ = GetRand(0.0f, PIx2);
	float lenRate = GetRand(radiusThickness_, 1.0f);

	Vector3D dirXZ = Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));

	float angle = (GetRand(0.0f, PIx2));

	dirXZ.Normalize();
	Vector3D v = Vector3D(0, 1, 0).cross(dirXZ);

	Quaternion q = SetQuaternion(v, angle);
	dir_ = RotateVector(dirXZ, q);
	dir_.Normalize();

	pos_ = parent_->GetPosition() + radius_ * dir_ * lenRate;

	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		obj->SetPosition(pos_);
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		sprite->SetPosition(pos_);
	}
}

void EmitterSphereType::SetRadius(float radius)
{
	radius_ = radius;
}

void EmitterSphereType::SetRadiusThickness(float thickness)
{
	radiusThickness_ = mClamp(0.0f, 1.0f, thickness);
}
