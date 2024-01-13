#include "EmitterConeType.h"
#include "MyMath.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

#include "Quaternion.h"

using namespace Easing;
using namespace MyMath;

void MNE::EmitterConeType::Update(MNE::Particle* particle)
{
	float angleXZ = GetRand(0.0f, PIx2);
	float lenRate = GetRand(0.0f, 1.0f);
	
	Vector3D dirXZ = Vector3D(cosf(angleXZ), 0.0f, sinf(angleXZ));

	float angle = ConvertToRad(angle_ - 90.0f);

	dirXZ.Normalize();
	Vector3D up(0, 1, 0);
	Vector3D v = up.cross(dirXZ);

	Quaternion q = SetQuaternion(v, angle);
	dir_ = RotateVector(dirXZ, q);
	dir_.Normalize();

	coneDir_.Normalize();
	Quaternion rotQ = DirectionToDirection(up, coneDir_);

	dir_ = Vec3Transform(dir_, rotQ.GetRotMatrix());
	dirXZ = Vec3Transform(dirXZ, rotQ.GetRotMatrix());
	pos_ = parent_->GetPosition() + radius_ * lenRate * dirXZ;

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
