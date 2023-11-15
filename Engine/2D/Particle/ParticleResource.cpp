#include "ParticleResource.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "TextureManager.h"
#include "ObjectParticle.h"
#include "ModelManager.h"

void ParticleResource::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();
		IModel* model = ModelManager::GetInstance()->GetModel(name_);
		obj->SetModel(model);
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();
		Texture* tex = TextureManager::GetInstance()->GetTextureGraph(name_);
		sprite->SetTextureHandle(tex->GetHandle());
	}
}

void ParticleResource::SetResourceName(const std::string& name)
{
	name_ = name;
}
