#include "Object3D.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "LightManager.h"
#include <cassert>
#include "CameraManager.h"
#include "IModel.h"
#include "DirectX.h"

#include "ConstBuffStruct.h"

void Object3D::SetModel(IModel* model)
{
	model_ = model;
}

void Object3D::SetCollider(BaseCollider* collider)
{
	collider->SetObject3D(this);
	std::unique_ptr<BaseCollider> coll(collider);
	this->collider_ = coll.get();
	CollisionManager::GetInstance()->AddCollider(std::move(coll));
	collider_->Update();
}

void Object3D::SetAttribute(unsigned short attribute)
{
	collider_->SetAttribute(attribute);
}

Object3D::~Object3D()
{
	if (collider_) {
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		collider_ = nullptr;
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

	dissolve_.Initialize(sizeof(CBuff::CBuffDissolveData));
	//	定数バッファのマッピング
	result = dissolve_.GetResource()->Map(0, nullptr, (void**)&cDissolveMap_);	//	マッピング
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
	if (collider_) {
		collider_->Update();
	}
}

void Object3D::MatUpdate(int32_t animationIdx)
{

#pragma region WorldMatrix
	mat_.Update();
#pragma endregion

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

	dissolveValue_ += 0.001f;
	if (dissolveValue_ > 1.0f) dissolveValue_ = 0.0f;
	cDissolveMap_->value = dissolveValue_;
	cDissolveMap_->color = { 2.0f,0.0f,0.0f,1.0f };

	//	Animation
	std::vector<Matrix> Transforms;
	//animationTimer_++;
	model_->BoneTransform(animationTimer_, Transforms, animationIdx);

	if (Transforms.empty()) return;
	for (size_t i = 0; i < model_->GetNumBones(); i++)
	{
		cSkinMap_->bones[i] = Transforms[i];
	}
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

	model_->Draw(1);
}

void Object3D::Draw(bool drawShadow)
{
	DrawShadow(drawShadow);
	
	DrawShadowReciever(drawShadow);

	DrawShadowUnReciever(drawShadow);
}

void Object3D::DrawShadow(bool drawShadow)
{
	if (!drawShadow) return;
	//	影を生成しないなら
	if (!shadowing_) return;

	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Shadow");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shadowTransform_.SetGraphicsRootCBuffView(2);
	lightMaterial_.SetGraphicsRootCBuffView(3);

	model_->Draw(1);
}

void Object3D::DrawShadowReciever(bool drawShadow)
{
	if (drawShadow) return;
	//	影の影響を受けないなら
	if (!shadowReciev_) return;

	GPipeline* pipeline_= PipelineManager::GetInstance()->GetPipeline("ShadowReciever");
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Texture* shadowmap = SceneManager::GetInstance()->GetShadowMap();
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(shadowmap->GetHandle()));
	
	transform_.SetGraphicsRootCBuffView(3);
	lightMaterial_.SetGraphicsRootCBuffView(4);
	skinData_.SetGraphicsRootCBuffView(5);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(6);

	model_->Draw(2);
}

void Object3D::DrawShadowUnReciever(bool drawShadow)
{
	if (drawShadow) return;
	//	影の影響を受けるなら
	if (shadowReciev_) return;

	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND);
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform_.SetGraphicsRootCBuffView(2);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(3);
	skinData_.SetGraphicsRootCBuffView(4);
	colorMaterial_.SetGraphicsRootCBuffView(5);

	model_->Draw(1);
}

void Object3D::DrawDissolve(bool drawShadow)
{
	if (drawShadow) return;

	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("dissolve");
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Texture* dissolve = TextureManager::GetInstance()->LoadTextureGraph("DissolveMap.png");
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(dissolve->GetHandle()));

	transform_.SetGraphicsRootCBuffView(3);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(4);
	skinData_.SetGraphicsRootCBuffView(5);
	colorMaterial_.SetGraphicsRootCBuffView(6);
	dissolve_.SetGraphicsRootCBuffView(7);

	model_->Draw(2);
}
