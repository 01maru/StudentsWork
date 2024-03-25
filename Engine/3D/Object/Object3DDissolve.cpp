#include "Object3DDissolve.h"
#include "ConstBuffStruct.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "RootParameterIdx.h"
#include "Shader.h"
#include <cassert>

using namespace MNE;

MNE::GPipeline* MNE::Object3DDissolve::sPipeline_ = nullptr;

void MNE::Object3DDissolve::Initialize()
{
	HRESULT result;
	TextureManager::GetInstance()->LoadTextureGraph("DissolveMap.png");
	dissolve_.Initialize(sizeof(MNE::CBuff::CBuffDissolveData));
	//	定数バッファのマッピング
	result = dissolve_.GetResource()->Map(0, nullptr, (void**)&cDissolveMap_);	//	マッピング
	assert(SUCCEEDED(result));

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

		Shader shader("DissolveVS", "DissolvePS");

		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 6, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, D3D12_DEPTH_WRITE_MASK_ALL,
			true, DXGI_FORMAT_R11G11B10_FLOAT, 2);

		sPipeline_ = pipeMan->AddPipeline(pipeline, "ModelDissolve");
	}

#pragma endregion

	Object3D::Initialize();
}

void MNE::Object3DDissolve::MatUpdate()
{
	Object3D::MatUpdate();

	cDissolveMap_->value = dissolveValue_;
	cDissolveMap_->color = dissolveColor_;
}

void MNE::Object3DDissolve::Draw()
{
	sPipeline_->SetGraphicsRootSignature();
	sPipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int32_t nextIdx = One;

	Texture* dissolve = TextureManager::GetInstance()->GetTextureGraph("DissolveMap.png");
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(nextIdx++, TextureManager::GetInstance()->GetTextureHandle(dissolve->GetHandle()));

	dissolve_.SetGraphicsRootCBuffView(nextIdx++);

	DrawModel(nextIdx);
}
