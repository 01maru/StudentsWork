//#include "ObjectParticle.h"
//#include "IModel.h"
//#include "PipelineManager.h"
//#include "ConstBuffStruct.h"
//#include "ICamera.h"
//
//void ObjectParticle::Initialize()
//{
//	mat_.Initialize();
//}
//
//void ObjectParticle::MatUpdate()
//{
//#pragma region WorldMatrix
//	mat_.Update();
//#pragma endregion
//
//	const Matrix& matViewProjection = sPtrCamera->GetViewProj();
//	const Vector3D& cameraPos = sPtrCamera->GetEye();
//
//	cTransformMap_->matViewProj = matViewProjection;
//	if (model_ != nullptr) {
//		cTransformMap_->matWorld = model_->GetModelTransform();
//		cTransformMap_->matWorld *= mat_.matWorld_;
//	}
//	else {
//		cTransformMap_->matWorld = mat_.matWorld_;
//	}
//	cTransformMap_->cameraPos = cameraPos;
//
//	cColorMap_->color = color_;
//}
//
//void ObjectParticle::Draw()
//{
//	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("Model", Blend::ALPHA_BLEND);
//	pipeline->SetGraphicsRootSignature();
//	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	transform_.SetGraphicsRootCBuffView(2);
//	colorMaterial_.SetGraphicsRootCBuffView(3);
//
//	model_->Draw(1);
//}
//
////-----------------------------------------------------------------------------
//// [SECTION] Getter
////-----------------------------------------------------------------------------
//
//const Vector4D& ObjectParticle::GetColor()
//{
//	return color_;
//}
//
////-----------------------------------------------------------------------------
//// [SECTION] Setter
////-----------------------------------------------------------------------------
//
//void ObjectParticle::SetModel(IModel* model)
//{
//	model_ = model;
//}
