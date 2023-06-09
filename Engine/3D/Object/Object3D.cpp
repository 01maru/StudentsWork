﻿#include "Object3D.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "PipelineManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "LightManager.h"
#include <cassert>
#include "CameraManager.h"
#include "IModel.h"
#include "DirectX.h"

#include "ConstBuffStruct.h"

GPipeline* Object3D::sPipeline = nullptr;

void Object3D::SetPipeline(GPipeline* pipeline_)
{
	Object3D::sPipeline = pipeline_;
}

void Object3D::SetModel(IModel* model)
{
	model_ = model;
}

void Object3D::SetCollider(BaseCollider* collider_)
{
	collider_->SetObject3D(this);
	this->collider = collider_;
	CollisionManager::GetInstance()->AddCollider(collider_);
	collider_->Update();
}

void Object3D::SetAttribute(unsigned short attribute)
{
	collider->SetAttribute(attribute);
}

Object3D::~Object3D()
{
	if (collider) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

Object3D* Object3D::Create(IModel* model_)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3D* obj = new Object3D();
	if (obj == nullptr) return nullptr;

	// 初期化
	obj->Initialize();

	if (model_) {
		obj->SetModel(model_);
	}

	return obj;
}

void Object3D::Initialize()
{
	HRESULT result;

#pragma region ConstBuff

	transform_.Initialize(sizeof(CBuff::CBuffObj3DTransform));
	//	定数バッファのマッピング
	result = transform_.GetResource()->Map(0, nullptr, (void**)&cTransformMap_);	//	マッピング
	assert(SUCCEEDED(result));

	shadowTransform_.Initialize(sizeof(CBuff::CBuffObj3DTransform));
	//	定数バッファのマッピング
	result = shadowTransform_.GetResource()->Map(0, nullptr, (void**)&cShadowTransMap_);	//	マッピング
	assert(SUCCEEDED(result));

	lightMaterial_.Initialize(sizeof(CBuff::CBuffLightMaterial));
	//	定数バッファのマッピング
	result = lightMaterial_.GetResource()->Map(0, nullptr, (void**)&cLightMap_);	//	マッピング
	assert(SUCCEEDED(result));

	skinData_.Initialize(sizeof(CBuff::CBuffSkinData));
	//	定数バッファのマッピング
	result = skinData_.GetResource()->Map(0, nullptr, (void**)&cSkinMap_);	//	マッピング
	assert(SUCCEEDED(result));

	colorMaterial_.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = colorMaterial_.GetResource()->Map(0, nullptr, (void**)&cColorMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

	mat_.Initialize();

	//	ボーンの初期化
	for (size_t i = 0; i < CBuff::MAX_BONES; i++)
	{
		cSkinMap_->bones[i] = Matrix();
	}
}

void Object3D::ColliderUpdate()
{
	if (collider) {
		collider->Update();
	}
}

void Object3D::MatUpdate()
{

#pragma region WorldMatrix
	mat_.Update();
#pragma endregion

	//if (isBillboard) {
	//	const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

	//	matWorld = XMMatrixIdentity();
	//	matWorld *= matScale; // ワールド行列にスケーリングを反映
	//	matWorld *= matRot; // ワールド行列に回転を反映
	//	matWorld *= matBillboard;
	//	matWorld *= matTrans; // ワールド行列に平行移動を反映
	//}

	// 親オブジェクトがあれば
	if (parent_ != nullptr) {
		mat_.matWorld_ *= parent_->mat_.matWorld_;
	}

	ICamera* cam = nullptr;
	if (camera_ != nullptr) {
		cam = camera_;
	}
	else {
		cam = CameraManager::GetInstance()->GetCamera();
	}
	const Matrix& matViewProjection = cam->GetViewProj();
	const Vector3D& cameraPos = cam->GetEye();

	cTransformMap_->matViewProj = matViewProjection;
	cTransformMap_->matViewProj = matViewProjection;
	if (model_ != nullptr) {
		cTransformMap_->matWorld = model_->GetModelTransform();
		cTransformMap_->matWorld *= mat_.matWorld_;
	}
	else {
		cTransformMap_->matWorld = mat_.matWorld_;
	}
	cTransformMap_->cameraPos = cameraPos;

	const Matrix& matView_ = CameraManager::GetInstance()->GetLightCamera()->GetViewProj();

	
	cShadowTransMap_->matViewProj = matView_;
	if (model_ != nullptr) {
		cShadowTransMap_->matWorld = model_->GetModelTransform();
		cShadowTransMap_->matWorld *= mat_.matWorld_;
	}
	else {
		cShadowTransMap_->matWorld = mat_.matWorld_;
	}

	cLightMap_->mLVP = matView_;
	cLightMap_->cameraPos = CameraManager::GetInstance()->GetLightCamera()->GetEye();

	cColorMap_->color = color_;
}

void Object3D::PlayAnimation()
{
	std::vector<Matrix> Transforms;

	animationTimer_ += 0.1f;
	model_->BoneTransform(animationTimer_, Transforms);

	for (size_t i = 0; i < model_->GetNumBones(); i++)
	{
		cSkinMap_->bones[i] = Transforms[i];
	}
}

void Object3D::Draw()
{
	sPipeline->SetGraphicsRootSignature();
	sPipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform_.SetGraphicsRootCBuffView(2);
	skinData_.SetGraphicsRootCBuffView(4);
	colorMaterial_.SetGraphicsRootCBuffView(5);

	LightManager::GetInstance()->SetGraphicsRootCBuffView(3);

	model_->Draw();
}

void Object3D::DrawSilhouette()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("ModelSilhouette");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform_.SetGraphicsRootCBuffView(2);
	skinData_.SetGraphicsRootCBuffView(4);
	colorMaterial_.SetGraphicsRootCBuffView(5);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(3);

	model_->Draw();
}

void Object3D::DrawShadow()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Shadow");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shadowTransform_.SetGraphicsRootCBuffView(2);
	lightMaterial_.SetGraphicsRootCBuffView(3);

	model_->Draw();
}

void Object3D::DrawShadowReciever()
{
	GPipeline* pipeline_= PipelineManager::GetInstance()->GetPipeline("ShadowReciever");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Texture* shadowmap = SceneManager::GetInstance()->GetShadowMap();
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(shadowmap->GetHandle()));

	transform_.SetGraphicsRootCBuffView(3);
	lightMaterial_.SetGraphicsRootCBuffView(4);
	skinData_.SetGraphicsRootCBuffView(5);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(6);

	model_->DrawShadowReciever();
}
