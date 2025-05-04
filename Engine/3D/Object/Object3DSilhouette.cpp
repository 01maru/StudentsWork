#include "Object3DSilhouette.h"
#include "PipelineManager.h"
#include "LightManager.h"
#include "IModel.h"
#include "RootParameterIdx.h"
#include "Shader.h"

MNE::GPipeline* MNE::Object3DSilhouette::sPipeline_ = nullptr;

void MNE::Object3DSilhouette::Initialize()
{
#pragma region Pipeline

	if (sPipeline_ == nullptr)
	{
		PipelineManager* pipeMan = PipelineManager::GetInstance();

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

		Shader shader("ObjVS", "SilhouettePS");

		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 5, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL,
			true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		pipeline->SetBlendMode(Blend::ALPHA_BLEND);

		sPipeline_ = pipeMan->AddPipeline(pipeline, "ModelSilhouette");
	}

#pragma endregion

	Object3D::Initialize();
}

void MNE::Object3DSilhouette::Draw()
{
	sPipeline_->SetGraphicsRootSignature();
	sPipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int32_t nextIdx = One;

	DrawModel(nextIdx);
}
