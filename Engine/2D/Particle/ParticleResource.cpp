#include "ParticleResource.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "TextureManager.h"

void ParticleResource::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();
		Texture* tex = TextureManager::GetInstance()->GetInstance()->GetTextureGraph(name_);
		sprite->SetTextureHandle(tex->GetHandle());
	}
}

void ParticleResource::SetResourceName(const std::string& name)
{
	name_ = name;
}
