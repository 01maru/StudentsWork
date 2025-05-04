#include "PipelineManager.h"
#include "Shader.h"

using namespace MNE;

MNE::PipelineManager* MNE::PipelineManager::GetInstance()
{
	static PipelineManager instance;
	return &instance;
}

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::PipelineManager::InitializeSprite()
{
#pragma region Sprite

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } ,	//	xyz座標
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	uv座標
	};

	Shader shader("SpriteVS", "SpritePS");

	for (int32_t i = 0; i < Blend::TOTAL_BLEND; i++)
	{
		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		pipeline->SetBlendMode(i);

		AddPipeline(pipeline, "Sprite", i);
	}

#pragma endregion
}

void MNE::PipelineManager::InitializeParticle()
{
#pragma region Particle

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
	};
	Shader shader("ParticleVS", "ParticlePS", "main", "ParticleGS");

	for (int32_t i = 0; i < Blend::TOTAL_BLEND; i++)
	{
		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		pipeline->SetBlendMode(i);

		AddPipeline(pipeline, "Particle", i);
	}
#pragma endregion
}

void MNE::PipelineManager::InitializePostEffect()
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } ,	//	xyz座標
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	uv座標
	};

	Shader shader("ScreenVS", "DepthOfFieldPS");
	
	std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(shader, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO,
		true, DXGI_FORMAT_R11G11B10_FLOAT, 2);

	AddPipeline(pipeline, "DoFPostEffect");
}

void MNE::PipelineManager::InitializeModel()
{
#pragma region Model

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } ,	//	xyz座標
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	法線ベクトル
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	uv座標
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	Shader shader;
	shader.Initialize("ObjVS", "ObjPS");

	for (int32_t i = 0; i < Blend::TOTAL_BLEND; i++)
	{
		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 5, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL,
			true, DXGI_FORMAT_R11G11B10_FLOAT);
		pipeline->SetBlendMode(i);

		AddPipeline(pipeline, "Model", i);
	}

#pragma endregion
}

void MNE::PipelineManager::Initialize()
{
	InitializeModel();

	InitializeSprite();

	InitializeParticle();
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void MNE::PipelineManager::ImGuiUpdate()
{
	for (auto& pipeline : pipelines_)
	{
		pipeline.second->ImGuiUpdate();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Add&Delete
//-----------------------------------------------------------------------------

bool MNE::PipelineManager::FindPipeline(const std::string& name)
{
	return pipelines_.count(name) != 0;
}

MNE::GPipeline* MNE::PipelineManager::AddPipeline(std::unique_ptr<GPipeline>& pipeline, const std::string& name)
{
	//	マップにパイプライン挿入
	auto pipe = pipelines_.emplace(name, std::move(pipeline));

	//	挿入できたら
	if (pipe.second == TRUE)
	{
		return 	pipe.first->second.get();
	}

	//	挿入できなかったら
	return nullptr;
}

GPipeline* MNE::PipelineManager::AddPipeline(std::unique_ptr<GPipeline>& pipeline, const std::string& name, int32_t blend)
{
	std::string blendName = name + "/B" + std::to_string(blend);

	return AddPipeline(pipeline, blendName);
}

void MNE::PipelineManager::DeletePipeline(const std::string& name)
{
	pipelines_.erase(name);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MNE::GPipeline* MNE::PipelineManager::GetPipeline(const std::string& name)
{
	//	指定したパイプラインがあればそれを返す
	if (pipelines_.count(name) != 0)
	{
		return pipelines_[name].get();
	}

	return nullptr;
}

MNE::GPipeline* MNE::PipelineManager::GetPipeline(const std::string& name, Blend::BlendMode blend)
{
	std::string blendName = name + "/B" + std::to_string(blend);

	return GetPipeline(blendName);
}
