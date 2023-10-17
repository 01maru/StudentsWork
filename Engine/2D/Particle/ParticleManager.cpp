#include "ParticleManager.h"
#include "TextureManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"

#include "Texture.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	//	画像読み込み
	particleTex_ = TextureManager::GetInstance()->LoadTextureGraph("particle2.png");
}

void ParticleManager::Update()
{
#pragma region Delete
	particles_.remove_if([](MoveParticle& x) {
		return x.GetIsEnd();
		});
#pragma endregion

	for (std::forward_list<MoveParticle>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		it->Update();
	}
}

void ParticleManager::MatUpdate()
{
	for (std::forward_list<MoveParticle>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		it->MatUpdate();
	}
}

void ParticleManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveParticleManager()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("ParticleManager");

	imgui->EndWindow();
}

void ParticleManager::Draw()
{
	for (std::forward_list<MoveParticle>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		it->Draw(particleTex_->GetHandle());
	}
}

void ParticleManager::AddMoveParticle(const Vector3D& pos, const Vector3D& spd, int time, float scale)
{
	particles_.emplace_front();
	MoveParticle& p = particles_.front();
	p.SetIsBillboard(true);
	p.SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
	p.Initialize(pos, spd, scale, time);
}

