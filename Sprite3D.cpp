#include "Sprite3D.h"
#include "TextureManager.h"
#include "ConstBuffStruct.h"
#include "CameraManager.h"
#include "RootParameterIdx.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "Shader.h"
#include <cassert>

using namespace MyMath;

void MNE::Sprite3D::Initialize(Texture* texture)
{
	HRESULT result;

	if (texture != nullptr) {
		handle_ = texture;
		AdjustTextureSize();
	}
	else {
		handle_ = TextureManager::GetWhiteTexture();
	}

#pragma region VertBuff

	vertices_.clear();
	vertices_.resize(VertexSize);

	SetVerticesPos();
	SetVerticesUV();
	
	indices_.push_back(LB);
	indices_.push_back(LT);
	indices_.push_back(RB);
	indices_.push_back(RB);
	indices_.push_back(LT);
	indices_.push_back(RT);

	uint32_t sizePV = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	VertIdxBuff::Initialize(sizePV, indices_);

#pragma endregion

#pragma region ConstBuffer

	cbTransform_.Initialize(sizeof(CBuff::CBuffObj3DTransform));
	//	定数バッファのマッピング
	result = cbTransform_.GetResource()->Map(0, nullptr, (void**)&cbTransformMap_);	//	マッピング
	assert(SUCCEEDED(result));


	cbColorMaterial_.Initialize(sizeof(CBuff::CBuffObjColorMaterial));
	//	定数バッファのマッピング
	result = cbColorMaterial_.GetResource()->Map(0, nullptr, (void**)&cbMaterialMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

	if (pipeline_ == nullptr)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } ,	//	xyz座標
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	uv座標
		};

		Shader shader;
		shader.Initialize("Sprite3DVS", "Sprite3DPS");

		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 2);

		pipeline_ = PipelineManager::GetInstance()->AddPipeline(pipeline, "Sprite3D");
	}
}

void MNE::Sprite3D::Update()
{
	mat_.Update();
	ICamera* cam = CameraManager::GetInstance()->GetCamera();
	cbTransformMap_->matViewProj = cam->GetViewProj();
	cbTransformMap_->matWorld = mat_.matTrans_;
	if (isBillboard_ == TRUE) {
		cbTransformMap_->matWorld *= cam->GetBillboard();
	}
	cbTransformMap_->cameraPos = cam->GetEye();

}

void MNE::Sprite3D::Draw()
{
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	IASetVertIdxBuff();
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	int32_t nextIdx = Zero;
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(nextIdx++, TextureManager::GetInstance()->GetTextureHandle(handle_->GetHandle()));

	cbColorMaterial_.SetGraphicsRootCBuffView(nextIdx++);
	cbTransform_.SetGraphicsRootCBuffView(nextIdx++);

	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void MNE::Sprite3D::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (size_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

void MNE::Sprite3D::SetVerticesUV()
{
	ID3D12Resource* texBuff = handle_->GetResourceBuff();

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();
		
		//float tex_left = textureLeftTop_.x / (float)resDesc.Width;
		//float tex_right = (textureLeftTop_.x + textureSize_.x) / (float)resDesc.Width;
		//float tex_top = textureLeftTop_.y / (float)resDesc.Height;
		//float tex_bottom = (textureLeftTop_.y + textureSize_.y) / (float)resDesc.Height;
		float tex_left = 0.0f;
		float tex_right = 1.0f;
		float tex_top = 0.0f;
		float tex_bottom = 1.0f;

		vertices_[LB].uv = { tex_left,tex_bottom };
		vertices_[LT].uv = { tex_left,tex_top };
		vertices_[RB].uv = { tex_right,tex_bottom };
		vertices_[RT].uv = { tex_right,tex_top };
	}
}

void MNE::Sprite3D::SetVerticesPos()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (1.0f - anchorPoint_.y) * size_.y;
	float bottom = (0.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	vertices_[LB].pos = { left,bottom,0.0f };
	vertices_[LT].pos = { left,top,0.0f };
	vertices_[RB].pos = { right,bottom,0.0f };
	vertices_[RT].pos = { right,top,0.0f };
}

void MNE::Sprite3D::AdjustTextureSize()
{
	ID3D12Resource* texBuff = handle_->GetResourceBuff();
	assert(texBuff);

	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();
	size_.x = static_cast<float>(resDesc.Width);
	size_.y = static_cast<float>(resDesc.Height);
}
