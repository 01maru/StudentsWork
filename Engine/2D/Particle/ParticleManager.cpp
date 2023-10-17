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
	//	パーティクルに使用する画像読み込み
	TextureManager::GetInstance()->LoadTextureGraph("particle2.png");
}

void ParticleManager::Update()
{
#pragma region Delete
	particles_.remove_if([](std::unique_ptr<IParticle>& x) {
		return x.get()->GetIsEnd();
		});
#pragma endregion

	for (std::forward_list<std::unique_ptr<IParticle>>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		it->get()->Update();
	}
}

void ParticleManager::MatUpdate()
{
	for (std::forward_list<std::unique_ptr<IParticle>>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		it->get()->MatUpdate();
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
	for (std::forward_list<std::unique_ptr<IParticle>>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		it->get()->Draw();
	}
}

void ParticleManager::AddParticle(std::unique_ptr<IParticle>& particle)
{
	particles_.push_front(std::move(particle));
}

