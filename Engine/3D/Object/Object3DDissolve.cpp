#include "Object3DDissolve.h"
#include "ConstBuffStruct.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "LightManager.h"
#include "IModel.h"
#include <cassert>

void Object3DDissolve::Initialize()
{
	HRESULT result;

	dissolve_.Initialize(sizeof(CBuff::CBuffDissolveData));
	//	定数バッファのマッピング
	result = dissolve_.GetResource()->Map(0, nullptr, (void**)&cDissolveMap_);	//	マッピング
	assert(SUCCEEDED(result));

	Object3D::Initialize();
}

void Object3DDissolve::MatUpdate()
{
	Object3D::MatUpdate();

	dissolveValue_ += 0.001f;
	if (dissolveValue_ > 1.0f) dissolveValue_ = 0.0f;
	cDissolveMap_->value = dissolveValue_;
	cDissolveMap_->color = { 2.0f,0.0f,0.0f,1.0f };
}

void Object3DDissolve::Draw()
{
	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("dissolve");
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Texture* dissolve = TextureManager::GetInstance()->GetTextureGraph("DissolveMap.png");
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(dissolve->GetHandle()));

	transform_.SetGraphicsRootCBuffView(3);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(4);
	animation_->SetGraphicsRootCBuffView(5);
	colorMaterial_.SetGraphicsRootCBuffView(6);
	dissolve_.SetGraphicsRootCBuffView(7);

	model_->Draw(2);
}
