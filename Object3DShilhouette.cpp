#include "Object3DShilhouette.h"
#include "PipelineManager.h"
#include "LightManager.h"
#include "IModel.h"

void Object3DShilhouette::Draw()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("ModelSilhouette");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform_.SetGraphicsRootCBuffView(2);
	animation_->SetGraphicsRootCBuffView(4);
	colorMaterial_.SetGraphicsRootCBuffView(5);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(3);

	model_->Draw(1);
}
