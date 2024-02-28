#include "EmitterType.h"
#include "ParticleEmitter.h"
#include "ObjectParticle.h"
#include "SpriteParticle.h"

using namespace MyMath;

//void MNE::EmitterType::Update(MNE::Particle* particle)
//{
//	pos_ = parent_->GetPosition();
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

std::vector<std::unique_ptr<MNE::Particle>> MNE::EmitterType::Update()
{
	std::vector<std::unique_ptr<MNE::Particle>> particles;

	for (int32_t i = 0; i < particleNum_; i++)
	{
		particles.emplace_back(std::make_unique<Particle>());
		Particle* particle = particles.back().get();

		Vector3D pos_ = parent_->GetPosition();
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

void MNE::EmitterType::SetParent(MNE::ParticleEmitter* parent)
{
	parent_ = parent;
}

void MNE::EmitterType::SetParticleNum(int32_t num)
{
	particleNum_ = num;
}
