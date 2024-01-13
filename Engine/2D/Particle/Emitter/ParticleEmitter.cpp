#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void MNE::ParticleEmitter::Update()
{
	//	アクティブじゃなかったら
	if (isActive_ == false) return;

	if (deadTimer_ != nullptr) {
		deadTimer_->Update();
		if (deadTimer_->GetIsActive() == false) {
			isDead_ = true;
		}
	}
	
	if (timer_.GetIsActive() == false) {

		timer_.StartCount();
		//	パーティクル生成
		AddParticle();
	}
	timer_.Update();
}

void MNE::ParticleEmitter::AddParticle()
{
	std::unique_ptr<Particle> particle = std::make_unique<Particle>();
	particle->StartTimer();
	particle->SetLifeTime(lifeTime_);
	particle->SetBlendMord(blendMord_);

	if (isObj_) {
		ObjectParticle* object = particle->AddComponent<ObjectParticle>();
		object->SetPosition(pos_);
	}
	else {
		SpriteParticle* sprite = particle->AddComponent<SpriteParticle>();
		sprite->SetPosition(pos_);
	}

	shapeType_->Update(particle.get());

	for (auto& compo : components_)
	{
		compo->Initialize(particle.get());
	}

	ParticleManager::GetInstance()->AddParticle(particle, isObj_);
}

bool MNE::ParticleEmitter::GetIsObj()
{
	return isObj_;
}

Vector3D& MNE::ParticleEmitter::GetPosition()
{
	return pos_;
}

void MNE::ParticleEmitter::SetIsObj(bool isObj)
{
	isObj_ = isObj;
}

void MNE::ParticleEmitter::SetRate(int32_t v)
{
	timer_.SetMaxFrameCount(v);
}

void MNE::ParticleEmitter::SetPosition(const Vector3D& pos)
{
	pos_ = pos;
}

void MNE::ParticleEmitter::SetLifeTime(int32_t lifeTime)
{
	lifeTime_ = lifeTime;
}

void MNE::ParticleEmitter::SetBlendMord(Blend::BlendMord blendMord)
{
	blendMord_ = blendMord;
}

void MNE::ParticleEmitter::SetEmitterType(std::unique_ptr<EmitterType>& type)
{
	shapeType_ = std::move(type);
	shapeType_->SetParent(this);
}

void MNE::ParticleEmitter::SetDeadTimer(std::unique_ptr<FrameCounter>& timer)
{
	deadTimer_ = std::move(timer);
	deadTimer_->StartCount();
}
