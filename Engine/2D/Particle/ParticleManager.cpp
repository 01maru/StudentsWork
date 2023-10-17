#include "ParticleManager.h"
#include "TextureManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"
#include "PipelineManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	//	パーティクルに使用する画像読み込み
	TextureManager::GetInstance()->LoadTextureGraph("particle.png");
	TextureManager::GetInstance()->LoadTextureGraph("particle2.png");

	//	パイプライン設定
	pipelines_.push_back(PipelineManager::GetInstance()->GetPipeline("Particle", Blend::ADD_BLEND));
	pipelines_.push_back(PipelineManager::GetInstance()->GetPipeline("Particle", Blend::SUB_BLEND));
	pipelines_.push_back(PipelineManager::GetInstance()->GetPipeline("Particle", Blend::INV_BLEND));
	pipelines_.push_back(PipelineManager::GetInstance()->GetPipeline("Particle", Blend::ALPHA_BLEND));
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
		//	パーティクルを動かす
		it->get()->Update();
	}
}

void ParticleManager::MatUpdate()
{
	for (std::forward_list<std::unique_ptr<IParticle>>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		//	パーティクルの行列更新
		it->get()->MatUpdate();
	}
}

void ParticleManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveParticleManager()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("ParticleManager");

#pragma region パーティクル用ImGui設定


#pragma endregion

	imgui->EndWindow();
}

void ParticleManager::Draw()
{
	Blend::BlendMord prev = Blend::NONE_BLEND;
	for (std::forward_list<std::unique_ptr<IParticle>>::iterator it = particles_.begin();
		it != particles_.end();
		it++) {
		//	リストの最初 or 前のブレンドモード設定から変更があったら
		if (it == particles_.begin() || prev != it->get()->GetBlendMord()) {
			GPipeline* pipeline = pipelines_[it->get()->GetBlendMord()];
			pipeline->SetGraphicsRootSignature();
			pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

			prev = it->get()->GetBlendMord();
		}

		//	パーティクル描画
		it->get()->Draw();
	}
}

void ParticleManager::AddParticle(std::unique_ptr<IParticle>& particle)
{
	//	パーティクルをリストに格納
	particles_.push_front(std::move(particle));
}

