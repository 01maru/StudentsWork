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

#include "LightCamera.h"

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
		collider_->Finalize();
		CollisionManager::GetInstance()->RemoveCollider();
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

	colorMaterial_.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = colorMaterial_.GetResource()->Map(0, nullptr, (void**)&cColorMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

	mat_.Initialize();

	shadow_ = std::make_unique<Object3DShadow>();
	shadow_->Initialize();
	shadow_->SetParent(this);

	animation_ = std::make_unique<Object3DAnimation>();
	animation_->Initialize();
}

void Object3D::ColliderUpdate()
{
	if (collider_) {
		collider_->Update();
	}
}

void Object3D::MatUpdate()
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

	shadow_->MatUpdate();

	cColorMap_->color = color_;

	animation_->MatUpdate();
}

void Object3D::Draw(bool drawShadow)
{
	DrawShadow(drawShadow);
	
	DrawShadowReciever(drawShadow);

	DrawShadowUnReciever(drawShadow);
}

void Object3D::Draw()
{
}

void Object3D::DrawShadow(bool drawShadow)
{
	if (!drawShadow) return;
	//	影を生成しないなら
	if (!shadowing_) return;

	shadow_->Draw();
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
	LightCamera* camera = dynamic_cast<LightCamera*>(CameraManager::GetInstance()->GetLightCamera());
	camera->SetGraphicsRootCBuffView(4);
	animation_->SetGraphicsRootCBuffView(5);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(6);

	model_->Draw(2);
}

void Object3D::DrawShadowUnReciever(bool drawShadow)
{
	if (drawShadow) return;
	//	影の影響を受けるなら
	if (shadowReciev_) return;

	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("Model", Blend::ALPHA_BLEND);
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform_.SetGraphicsRootCBuffView(2);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(3);
	animation_->SetGraphicsRootCBuffView(4);
	colorMaterial_.SetGraphicsRootCBuffView(5);

	model_->Draw(1);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Object3D::SetModel(IModel* model)
{
	model_ = model;
	animation_->SetModel(model);
}
