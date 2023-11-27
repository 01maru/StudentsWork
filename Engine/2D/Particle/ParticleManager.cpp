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

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void ParticleManager::DeleteParticle()
{
	spriteParticles_.remove_if([](std::unique_ptr<Particle>& x) {
		return x->GetIsEnd();
		});

	objParticles_.remove_if([](std::unique_ptr<Particle>& x) {
		return x->GetIsEnd();
		});

	emitters_.remove_if([](std::unique_ptr<ParticleEmitter>& x) {
		return x->GetIsDead();
		});
}

void ParticleManager::Update()
{
	DeleteParticle();

	//	スプライトパーティクル更新
	for (auto& particle : spriteParticles_)
	{
		particle->Update();
	}
	//	オブジェクトパーティクル更新
	for (auto& particle : objParticles_)
	{
		particle->Update();
	}

	//	エミッター更新
	for (auto itr = emitters_.begin(); itr != emitters_.end(); itr++)
	{
		itr->get()->Update();
	}
}

void ParticleManager::MatUpdate()
{
	for (auto& particle : spriteParticles_) {
		//	パーティクルの行列更新
		particle->MatUpdate();
	}
	for (auto& particle : objParticles_) {
		//	パーティクルの行列更新
		particle->MatUpdate();
	}
}

void ParticleManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveParticleManager()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("ParticleManager");

#pragma region パーティクル用ImGui設定

	//if(imgui->SetButton("AddParticle")) {
	//}

#pragma endregion

	imgui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void ParticleManager::Draw()
{
	Blend::BlendMord prev = Blend::NONE_BLEND;

	for (auto itr = spriteParticles_.begin(); itr != spriteParticles_.end(); itr++)
	{
		//	リストの最初 or 前のブレンドモード設定から変更があったら
		if (itr == spriteParticles_.begin() || prev != itr->get()->GetBlendMord()) {
			GPipeline* pipeline = pipelines_[itr->get()->GetBlendMord()];
			pipeline->SetGraphicsRootSignature();
			pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

			prev = itr->get()->GetBlendMord();
		}

		//	パーティクル描画
		itr->get()->Draw();
	}

	for (auto itr = objParticles_.begin(); itr != objParticles_.end(); itr++)
	{
		//	パーティクル描画
		itr->get()->Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Delete
//-----------------------------------------------------------------------------

void ParticleManager::DeleteAllParticle()
{
	spriteParticles_.clear();

	objParticles_.clear();

	emitters_.clear();
}

//-----------------------------------------------------------------------------
// [SECTION] Add
//-----------------------------------------------------------------------------

void ParticleManager::AddParticle(std::unique_ptr<Particle>& particle, bool isObj)
{
	if (isObj) {
		objParticles_.push_front(std::move(particle));

		return;
	}

	else {
		spriteParticles_.push_front(std::move(particle));

		return;
	}
}

ParticleEmitter* ParticleManager::AddEmitter(std::unique_ptr<ParticleEmitter>& emitter)
{
	emitters_.push_front(std::move(emitter));
	return emitters_.front().get();
}
