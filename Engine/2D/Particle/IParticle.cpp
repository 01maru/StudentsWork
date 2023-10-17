#include "IParticle.h"
#include "TextureManager.h"
#include "DirectX.h"
#include <cassert>
#include "ConstBuffStruct.h"
#include "CameraManager.h"

void IParticle::TransferVertex()
{
	//	GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vector3D* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//	座標をコピー
	vertMap[0] = vertex_;

	//	繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

void IParticle::SetVertices()
{
	//	頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertex_);

	//	GPUメモリの値書き換え
	TransferVertex();
}

void IParticle::Initialize()
{
	HRESULT result;
	
#pragma region  ConstBuffer

	transform_.Initialize(sizeof(CBuff::CBufferParticleTransform));
	//	定数バッファのマッピング
	result = transform_.GetResource()->Map(0, nullptr, (void**)&cTransformMap_);	//	マッピング
	assert(SUCCEEDED(result));

	colorMaterial_.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = colorMaterial_.GetResource()->Map(0, nullptr, (void**)&cColorMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region VertBuff

	//	頂点バッファの初期化
	uint16_t sizePV = static_cast<uint16_t>(sizeof(vertex_));
	VertIdxBuff::Initialize(sizePV);

#pragma endregion
}

void IParticle::MatUpdate()
{
	ICamera* camera = CameraManager::GetInstance()->GetCamera();

	cTransformMap_->matBillboard = Matrix();
	if (isBillboardY_) {
		cTransformMap_->matBillboard = camera->GetBillboardY();
	}
	if (isBillboard_) {
		cTransformMap_->matBillboard = camera->GetBillboard();
	}
	cTransformMap_->scale = scale_;
	cTransformMap_->cameraPos = camera->GetEye();
	cTransformMap_->mat = camera->GetViewProj();

	cColorMap_->color = color_;
}

void IParticle::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//	頂点バッファ設定
	IASetVertIdxBuff();
	//	テクスチャ設定
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texHandle_));

	//	定数バッファ設定
	colorMaterial_.SetGraphicsRootCBuffView(1);
	transform_.SetGraphicsRootCBuffView(2);

	//	描画
	cmdList->DrawInstanced(1, 1, 0, 0);
}

void IParticle::SetPosition(const Vector3D& pos)
{
	vertex_ = pos;
	TransferVertex();
}