#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void ParticleEmitter::Update()
{
	//	アクティブじゃなかったら
	if (isActive_ == false) return;

	if (timer_++ >= rate_) {
		timer_ = 0;

		//	パーティクル生成
		AddParticle();
	}
}

void ParticleEmitter::AddParticle()
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

	for (auto& compo : components_)
	{
		compo->Initialize(particle.get());
	}

	ParticleManager::GetInstance()->AddParticle(particle, isObj_);
}

bool ParticleEmitter::GetIsObj()
{
	return isObj_;
}

Vector3D& ParticleEmitter::GetPosition()
{
	return pos_;
}

void ParticleEmitter::SetIsObj(bool isObj)
{
	isObj_ = isObj;
}

void ParticleEmitter::SetRate(int32_t v)
{
	rate_ = v;
}

void ParticleEmitter::SetPosition(const Vector3D& pos)
{
	pos_ = pos;
}

void ParticleEmitter::SetLifeTime(int32_t lifeTime)
{
	lifeTime_ = lifeTime;
}

void ParticleEmitter::SetBlendMord(Blend::BlendMord blendMord)
{
	blendMord_ = blendMord;
}
