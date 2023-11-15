#include "SpriteParticle.h"
#include "ConstBuffStruct.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "ICamera.h"
#include <cassert>
#include "Particle.h"

#include "CameraManager.h"
#include "RootParameterIdx.h"

void SpriteParticle::Initialize()
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

void SpriteParticle::MatUpdate()
{
	ICamera* camera = CameraManager::GetInstance()->GetMainCamera();
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

void SpriteParticle::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//	頂点バッファ設定
	IASetVertIdxBuff();

	int32_t nextIdx = Zero;
	
	//	テクスチャ設定
	cmdList->SetGraphicsRootDescriptorTable(nextIdx++, TextureManager::GetInstance()->GetTextureHandle(texHandle_));

	//	定数バッファ設定
	colorMaterial_.SetGraphicsRootCBuffView(nextIdx++);
	transform_.SetGraphicsRootCBuffView(nextIdx++);

	//	描画
	cmdList->DrawInstanced(1, 1, 0, 0);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

const Vector3D& SpriteParticle::GetPosition()
{
	return vertex_;
}

const Vector4D& SpriteParticle::GetColor()
{
	return color_;
}

float SpriteParticle::GetScale()
{
	return scale_;
}

bool SpriteParticle::GetIsBillboard()
{
	return isBillboard_;
}

bool SpriteParticle::GetIsBillboardY()
{
	return isBillboardY_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SpriteParticle::SetTextureHandle(int32_t handle)
{
	texHandle_ = handle;
}

void SpriteParticle::SetPosition(const Vector3D& pos)
{
	if (vertex_ == pos) return;

	vertex_ = pos;
	TransferVertex();
}

void SpriteParticle::SetColor(const Vector4D& color)
{
	color_ = color;
}

void SpriteParticle::SetScale(float scale)
{
	scale_ = scale;
}

void SpriteParticle::SetIsBillboard(bool isBillboard)
{
	isBillboard_ = isBillboard;
}

void SpriteParticle::SetIsBillboardY(bool isBillboardY)
{
	isBillboardY_ = isBillboardY;
}

void SpriteParticle::TransferVertex()
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

void SpriteParticle::SetVertices()
{
	//	頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertex_);

	//	GPUメモリの値書き換え
	TransferVertex();
}
