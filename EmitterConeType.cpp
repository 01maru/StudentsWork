#include "EmitterConeType.h"
#include "MyMath.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

#include "Quaternion.h"

using namespace Easing;
using namespace MyMath;

void EmitterConeType::Update(Particle* particle)
{
	float angleXZ = GetRand(0.0f, PIx2);
	float lenRate = GetRand(0.0f, 1.0f);
	
	Vector3D dirXZ = Vector3D(radius_* lenRate * cos(angleXZ), 0.0f, radius_ * lenRate * sinf(angleXZ));
	pos_ = parent_->GetPosition() + dirXZ;

	//float angle = 90.0f - lerp(0.0f, angle_, lenRate);
	float angle = ConvertToRad(90.0f - angle_);

	//Vector3D v = Vector3D(0, 1, 0).cross(dirXZ);
	
	dirXZ.Normalize();
	Vector3D v = Vector3D(0, -1, 0).cross(dirXZ);

	Quaternion q = SetQuaternion(v, angle);
	dir_ = RotateVector(dirXZ, q);
	dir_.Normalize();

	//dir_ = Vector3D(0, 1, 0);

	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		//Vector3D pos = obj->GetPosition();
		obj->SetPosition(pos_);
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		//Vector3D pos = sprite->GetPosition();
		//pos += add_->GetValue();
		sprite->SetPosition(pos_);
	}
}
