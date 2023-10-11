#include "ParticleManager.h"
#include "TextureManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	//particleTex_ = TextureManager::GetInstance()->LoadTextureGraph("particle2.png");
}

void ParticleManager::Update()
{
#pragma region Delete
	particles.remove_if([](MoveParticle& x) {
		return x.GetIsEnd();
		});
#pragma endregion

	for (std::forward_list<MoveParticle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		it->Update();
	}
}

void ParticleManager::MatUpdate()
{
	for (std::forward_list<MoveParticle>::iterator it = particles.begin();
		it != particles.end();
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
	for (std::forward_list<MoveParticle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		it->Draw(particleTex_->GetHandle());
	}
}

void ParticleManager::AddMoveParticle(const Vector3D& pos, const Vector3D& spd, int time, float scale)
{
	particles.emplace_front();
	MoveParticle& p = particles.front();
	p.SetIsBillboard(true);
	p.SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
	p.Initialize(pos, spd, scale, time);
}

