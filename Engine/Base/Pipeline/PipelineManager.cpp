#include "PipelineManager.h"
#include "Shader.h"

PipelineManager* PipelineManager::GetInstance()
{
	static PipelineManager instance;
	return &instance;
}

void PipelineManager::InitializeSprite()
{
#pragma region Sprite
	//	Nonblend以外
	spritePipeline_.reserve(4);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.emplace_back();
	inputLayout.back() = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };	//	xyz座標
	inputLayout.emplace_back();
	inputLayout.back() = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };					//	uv座標

	Shader shader(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");

	for (size_t i = 0; i < 4; i++)
	{
		GPipeline* spritepipe_ = new GPipeline();
		spritepipe_->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		spritepipe_->SetBlendMord(i);

		//	pipeline追加
		spritePipeline_.emplace_back(spritepipe_);
	}

	loadingSpritePipe_ = std::make_unique<GPipeline>();
	loadingSpritePipe_->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	loadingSpritePipe_->SetBlendMord(Blend::ALPHA_BLEND);

	Shader dissolveShader(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/DissolveSpritePS.hlsl");

	dissolveSpritePipe_ = std::make_unique<GPipeline>();
	dissolveSpritePipe_->Initialize(dissolveShader, inputLayout, 3, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 2);
	dissolveSpritePipe_->SetBlendMord(Blend::ALPHA_BLEND);
#pragma endregion
}

void PipelineManager::InitializeParticle()
{
#pragma region Particle
	particlePipeline_.reserve(4);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.emplace_back();
	inputLayout.back() = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };	//	xyz座標

	Shader shader(L"Resources/Shader/ParticleVS.hlsl", L"Resources/Shader/ParticlePS.hlsl", "main", L"Resources/Shader/ParticleGS.hlsl");

	for (size_t i = 0; i < 4; i++)
	{
		GPipeline* particlepipe_ = new GPipeline();
		particlepipe_->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		particlepipe_->SetBlendMord(i);

		//	pipeline追加
		particlePipeline_.emplace_back(particlepipe_);
	}
#pragma endregion
}

void PipelineManager::InitializePostEffect()
{
#pragma region PostEffect

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.emplace_back();
	inputLayout.back() = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };	//	xyz座標
	inputLayout.emplace_back();
	inputLayout.back() = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };					//	uv座標

	Shader luminnceShader(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/ScreenPS.hlsl");
	
	postEffectPipeline_ = std::make_unique<GPipeline>();
	postEffectPipeline_->Initialize(luminnceShader, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R11G11B10_FLOAT, 4);

	Shader postEffect(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/ScreenShadowPS.hlsl");

	postShadowPipeline_ = std::make_unique<GPipeline>();
	postShadowPipeline_->Initialize(postEffect, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);

	Shader glayShader(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/GlayScalePS.hlsl");
	
	glayScalePipeline_ = std::make_unique<GPipeline>();
	glayScalePipeline_->Initialize(glayShader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

	Shader dofShader(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/DepthOfFieldPS.hlsl");
	
	dofPipeline_ = std::make_unique<GPipeline>();
	dofPipeline_->Initialize(dofShader, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R11G11B10_FLOAT, 2);

	Shader luminncePostEffect(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/LuminncePS.hlsl");
	
	luminncePipeline_ = std::make_unique<GPipeline>();
	luminncePipeline_->Initialize(luminncePostEffect, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);

#pragma region Blur
	Shader xblur(L"Resources/Shader/XBlurVS.hlsl", L"Resources/Shader/BlurPS.hlsl");

	xBlurPipeline_ = std::make_unique<GPipeline>();
	xBlurPipeline_->Initialize(xblur, inputLayout
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R32G32_FLOAT);

	Shader yblur(L"Resources/Shader/YBlurVS.hlsl", L"Resources/Shader/BlurPS.hlsl");

	yBlurPipeline_ = std::make_unique<GPipeline>();
	yBlurPipeline_->Initialize(yblur, inputLayout
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R32G32_FLOAT);

	luminncexBlurPipeline_ = std::make_unique<GPipeline>();
	luminncexBlurPipeline_->Initialize(xblur, inputLayout
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true);

	luminnceyBlurPipeline_ = std::make_unique<GPipeline>();
	luminnceyBlurPipeline_->Initialize(yblur, inputLayout
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true);
#pragma endregion
}

void PipelineManager::InitializeModel()
{
	size_t blendMordNum = 5;

#pragma region Model

	Shader shadowShader(L"Resources/Shader/ObjShadowVS.hlsl", L"Resources/Shader/ObjShadowPS.hlsl");

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.emplace_back();
	inputLayout.back() = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };	//	xyz座標

	shadowPipeline_ = std::make_unique<GPipeline>();
	shadowPipeline_->Initialize(shadowShader, inputLayout, 5
		, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R32G32_FLOAT);
	shadowPipeline_->SetBlendMord(Blend::NONE_BLEND);

	inputLayout.emplace_back();
	inputLayout.back() = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };		//	法線ベクトル
	inputLayout.emplace_back();
	inputLayout.back() = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };					//	uv座標

	inputLayout.emplace_back();
	inputLayout.back() = { "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputLayout.emplace_back();
	inputLayout.back() = { "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	Shader shadowRecieverShader(L"Resources/Shader/ShadowRecieverVS.hlsl", L"Resources/Shader/ShadowRecieverPS.hlsl");

	shadowRecieverPipeline_ = std::make_unique<GPipeline>();
	shadowRecieverPipeline_->Initialize(shadowRecieverShader, inputLayout
		, 6, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R11G11B10_FLOAT, 2);
	shadowRecieverPipeline_->SetBlendMord(Blend::NONE_BLEND);

	Shader silhouetteShader(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/SilhouettePS.hlsl");

	modelPipeline_.reserve(blendMordNum);
	Shader objShader(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");

	modelSilhouettePipe_ = std::make_unique<GPipeline>();
	modelSilhouettePipe_->Initialize(silhouetteShader, inputLayout, 5,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	modelSilhouettePipe_->SetBlendMord(Blend::ALPHA_BLEND);

	for (size_t i = 0; i < blendMordNum; i++)
	{
		//	pipeline追加
		modelPipeline_.emplace_back(new GPipeline());

		GPipeline* modelpipeline_ = modelPipeline_.back().get();
		modelpipeline_->Initialize(objShader, inputLayout, 5
			, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true,
			DXGI_FORMAT_R11G11B10_FLOAT);
		modelpipeline_->SetBlendMord(i);
	}

	Shader dissolveShader(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/DissolvePS.hlsl");

	pipelines_.emplace("dissolve", std::make_unique<GPipeline>());
	GPipeline* dissolvePipe = pipelines_["dissolve"].get();
	dissolvePipe->Initialize(dissolveShader, inputLayout, 6
		, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, D3D12_DEPTH_WRITE_MASK_ALL, true,
		DXGI_FORMAT_R11G11B10_FLOAT, 2);

#pragma endregion
}

void PipelineManager::Initialize()
{
	InitializeModel();

	InitializePostEffect();

	InitializeSprite();

	InitializeParticle();
}

GPipeline* PipelineManager::GetPipeline(const std::string& name, Blend::BlendMord blend)
{
	if (name == "Model") {
		return modelPipeline_[blend].get();
	}
	else if (name == "Shadow") {
		return shadowPipeline_.get();
	}
	else if (name == "ShadowReciever") {
		return shadowRecieverPipeline_.get();
	}
	else if (name == "ModelSilhouette") {
		return modelSilhouettePipe_.get();
	}
	else if (name == "Sprite") {
		return spritePipeline_[blend].get();
	}
	else if (name == "LoadingSprite") {
		return loadingSpritePipe_.get();
	}
	else if (name == "DissolveSprite") {
		return dissolveSpritePipe_.get();
	}
	else if (name == "Particle") {
		return particlePipeline_[blend].get();
	}
	else if (name == "PostEffect") {
		return postEffectPipeline_.get();
	}
	else if (name == "Luminnce") {
		return luminncePipeline_.get();
	}
	else if (name == "PostEffectShadow") {
		return postShadowPipeline_.get();
	}
	else if (name == "xBlur") {
		return xBlurPipeline_.get();
	}
	else if (name == "yBlur") {
		return yBlurPipeline_.get();
	}
	else if (name == "luminncexBlur") {
		return luminncexBlurPipeline_.get();
	}
	else if (name == "luminnceyBlur") {
		return luminnceyBlurPipeline_.get();
	}
	else if (name == "glayScale") {
		return glayScalePipeline_.get();
	}
	else if (name == "dissolve") {
		return pipelines_[name].get();
	}
	return nullptr;
}
