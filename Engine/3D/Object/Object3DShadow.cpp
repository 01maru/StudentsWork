#include "Object3DShadow.h"
#include "ConstBuffStruct.h"
#include <cassert>

#include "Object3D.h"
#include "CameraManager.h"
#include "LightCamera.h"
#include "PipelineManager.h"
#include "RootParameterIdx.h"

void MNE::Object3DShadow::Initialize()
{
	HRESULT result;

	shadowTransform_.Initialize(sizeof(MNE::CBuff::CBuffObj3DTransform));
	//	定数バッファのマッピング
	result = shadowTransform_.GetResource()->Map(0, nullptr, (void**)&cShadowTransMap_);	//	マッピング
	assert(SUCCEEDED(result));
}

void MNE::Object3DShadow::MatUpdate()
{
	const Matrix& matView_ = CameraManager::GetInstance()->GetLightCamera()->GetViewProj();

	cShadowTransMap_->matViewProj = matView_;

	IModel* model = parent_->GetModel();
	if (model != nullptr) {
		cShadowTransMap_->matWorld = model->GetModelTransform();
		cShadowTransMap_->matWorld *= parent_->GetMatWorld();
	}
	else {
		cShadowTransMap_->matWorld = parent_->GetMatWorld();
	}
}

void MNE::Object3DShadow::Draw()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Shadow");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int32_t nextIdx = One;

	shadowTransform_.SetGraphicsRootCBuffView(nextIdx++);
	LightCamera* camera = dynamic_cast<LightCamera*>(CameraManager::GetInstance()->GetLightCamera());
	camera->SetGraphicsRootCBuffView(nextIdx++);

	parent_->GetModel()->Draw(nextIdx++);
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::Object3DShadow::SetParent(Object3D* parent)
{
	parent_ = parent;
}
