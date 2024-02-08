#include "Bonfire.h"
#include "ParticleManager.h"
#include "FireParticleEmitter.h"
#include "SmokeParticleEmitter.h"

using namespace MNE;

void Bonfire::Start()
{
	//	煙エミッターの生成
	if (smoke_ == nullptr) {
		SmokeParticleEmitter smokeEmitter;
		smoke_ = ParticleManager::GetInstance()->AddEmitter(smokeEmitter.GetEmitter());
	}

	//	炎エミッターの生成
	if (fire_ == nullptr) {
		FireParticleEmitter emitter;
		fire_ = ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());
	}
}

void Bonfire::Stop()
{
	if (smoke_ != nullptr) {
		smoke_->SetIsDead(true);
		smoke_ = nullptr;
	}
	if (fire_ != nullptr) {
		fire_->SetIsDead(true);
		fire_ = nullptr;
	}
}
