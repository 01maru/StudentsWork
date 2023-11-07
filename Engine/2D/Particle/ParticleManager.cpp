#include "ParticleManager.h"
#include "TextureManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"
#include "PipelineManager.h"
#include "SpriteParticle.h"

#include "CameraManager.h"
#include "EmitterScaleAnimation.h"
#include "ParticleResource.h"
#include "ParticleValue.h"
#include "EmitterPosComp.h"
#include "EmitterSpdAnimation.h"

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

	std::unique_ptr<ParticleEmitter> emitter_ = std::make_unique<ParticleEmitter>();
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleRandValue> v = std::make_unique<ParticleRandValue>();
	v->SetMinValue(Vector3D{ 1,0,0 });
	v->SetMaxValue(Vector3D{ 5,0,0 });
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetValue(v1);
	std::unique_ptr<ParticleRandValue> addV = std::make_unique<ParticleRandValue>();
	addV->SetMinValue(Vector3D{ -5,0,-5 });
	addV->SetMaxValue(Vector3D{ 5,0,5 });
	std::unique_ptr<ParticleValue> v2 = std::move(addV);
	//emitter_->SetPosition({-0.0f,5.0f,0.0f });
	EmitterPosComp* pos = emitter_->AddComponent<EmitterPosComp>();
	pos->SetAddValue(v2);
	emitter_->SetBlendMord(Blend::ADD_BLEND);
	emitter_->AddComponent<EmitterSpdAnimation>();

	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle.png");

	emitters_.push_front(std::move(emitter_));
}

void ParticleManager::Update()
{
#pragma region Delete
	particles_.remove_if([](std::unique_ptr<Particle>& x) {
		return x.get()->GetIsEnd();
		});
#pragma endregion

	for (auto& particle : particles_)
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
	for (auto& particle : particles_) {
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

void ParticleManager::Draw()
{
	Blend::BlendMord prev = Blend::NONE_BLEND;

	for (auto itr = particles_.begin(); itr != particles_.end(); itr++)
	{
		//	リストの最初 or 前のブレンドモード設定から変更があったら
		if (itr == particles_.begin() || prev != itr->get()->GetBlendMord()) {
			GPipeline* pipeline = pipelines_[itr->get()->GetBlendMord()];
			pipeline->SetGraphicsRootSignature();
			pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

			prev = itr->get()->GetBlendMord();
		}

		//	パーティクル描画
		itr->get()->Draw();
	}
}

void ParticleManager::AddParticle(std::unique_ptr<Particle>& particle, bool isObj)
{
	if (isObj) {
		objParticles_.push_front(std::move(particle));

		return;
	}

	else {
		particles_.push_front(std::move(particle));

		return;
	}
}
