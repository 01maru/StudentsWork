#include "ParticleResource.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "TextureManager.h"
#include "ObjectParticle.h"
#include "ModelManager.h"

void MNE::ParticleResource::Initialize(MNE::Particle* particle)
{
	//	オブジェクトだったら
	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();
		IModel* model = ModelManager::GetInstance()->GetModel(name_);
		obj->SetModel(model);
	}

	//	スプライトだったら
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();
		Texture* tex = TextureManager::GetInstance()->GetTextureGraph(name_);
		sprite->SetTextureHandle(tex->GetHandle());
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::ParticleResource::SetResourceName(const std::string& name)
{
	name_ = name;
}
