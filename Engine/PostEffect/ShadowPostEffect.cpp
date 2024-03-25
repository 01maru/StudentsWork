#include "ShadowPostEffect.h"
#include "DirectX.h"
#include "SceneManager.h"
#include "PipelineManager.h"
#include "Shader.h"

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::ShadowPostEffect::Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum, DXGI_FORMAT format)
{
#pragma region Pipeline

	PipelineManager* pipeMan = PipelineManager::GetInstance();
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }		//	uv座標
	};

	//	ShadowMap
	Shader shader("ScreenVS", "ScreenShadowPS");

	std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(shader, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);

	pipeline_ = pipeMan->AddPipeline(pipeline, "ShadowMap");

	//	ObjShadow
	std::vector<D3D12_INPUT_ELEMENT_DESC> objInputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
	};

	if (pipeMan->FindPipeline("Shadow") == FALSE)
	{
		Shader shadowShader("ObjShadowVS", "ObjShadowPS");

		pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shadowShader, inputLayout, 5, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL,
			true, DXGI_FORMAT_R32G32_FLOAT);
		pipeline->SetBlendMord(Blend::NONE_BLEND);

		pipeMan->AddPipeline(pipeline, "Shadow");
	}

	//	ObjShadowReceiver

	if (pipeMan->FindPipeline("ShadowReceiver") == FALSE)
	{
		Shader receiverShader("ShadowReceiverVS", "ShadowReceiverPS");

		objInputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });	//	法線ベクトル

		objInputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 });	//	uv座標

		objInputLayout.push_back({ "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 });

		objInputLayout.push_back({ "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 });

		pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(receiverShader, objInputLayout, 6, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL,
			true, DXGI_FORMAT_R11G11B10_FLOAT, 2);
		pipeline->SetBlendMord(Blend::NONE_BLEND);

		pipeMan->AddPipeline(pipeline, "ShadowReceiver");
	}

#pragma endregion

	IPostEffect::Initialize(width, height, name, textureNum, format);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::ShadowPostEffect::Update()
{
	MyDirectX* dx = MyDirectX::GetInstance();

	dx->PrevPostEffect(this, clearColor_);

	SceneManager::GetInstance()->DrawShadow();

	if (originalPE_ != nullptr)
	{
		originalPE_->Draw();
	}

	dx->PostEffectDraw(this);
}
