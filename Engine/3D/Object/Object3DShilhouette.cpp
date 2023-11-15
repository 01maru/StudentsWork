#include "Object3DShilhouette.h"
#include "PipelineManager.h"
#include "LightManager.h"
#include "IModel.h"
#include "RootParameterIdx.h"

void Object3DShilhouette::Draw()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("ModelSilhouette");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int32_t nextIdx = One;

	DrawModel(nextIdx);
}
