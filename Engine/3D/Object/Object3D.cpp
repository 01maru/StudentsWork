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

#include "RootParameterIdx.h"

Object3D::~Object3D()
{
	if (collider_) {
		collider_->Finalize();
		CollisionManager::GetInstance()->RemoveCollider();
		collider_ = nullptr;
	}
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

void Object3D::MatUpdate(bool calcRot)
{
#pragma region WorldMatrix
	mat_.Update(calcRot);
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

void Object3D::DrawShadow()
{
	//	影を生成しないなら
	if (!shadowing_) return;

	shadow_->Draw();
}

void Object3D::DrawModel(int32_t& rootParaIdx)
{
	transform_.SetGraphicsRootCBuffView(rootParaIdx++);
	LightManager::GetInstance()->SetGraphicsRootCBuffView(rootParaIdx++);
	animation_->SetGraphicsRootCBuffView(rootParaIdx++);
	colorMaterial_.SetGraphicsRootCBuffView(rootParaIdx++);

	model_->Draw(rootParaIdx++);
}

void Object3D::DrawShadowReciever(int32_t& nextIdx)
{
	//	影の影響を受けないなら
	if (!shadowReciev_) return;

	Texture* shadowmap = SceneManager::GetInstance()->GetShadowMap();
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(nextIdx++, TextureManager::GetInstance()->GetTextureHandle(shadowmap->GetHandle()));

	LightCamera* camera = dynamic_cast<LightCamera*>(CameraManager::GetInstance()->GetLightCamera());
	camera->SetGraphicsRootCBuffView(nextIdx++);
}

void Object3D::Draw()
{
	if (SceneManager::GetInstance()->GetIsDrawShadow() == true) {

		DrawShadow();

	}
	else {
		int32_t nextIdx = One;

		GPipeline* pipeline = nullptr;
		if (shadowReciev_ == true) {
			pipeline = PipelineManager::GetInstance()->GetPipeline("ShadowReciever");
		}
		else {
			pipeline = PipelineManager::GetInstance()->GetPipeline("Model", Blend::ALPHA_BLEND);
		}
		pipeline->SetGraphicsRootSignature();
		pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DrawShadowReciever(nextIdx);

		DrawModel(nextIdx);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

std::unique_ptr<Object3D> Object3D::Create(IModel* model_)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<Object3D> obj = std::make_unique<Object3D>();
	if (obj == nullptr) return obj;

	// 初期化
	obj->Initialize();

	if (model_ != nullptr) {
		obj->SetModel(model_);
	}

	return obj;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Object3D::SetCollider(BaseCollider* collider)
{
	collider->SetObject3D(this);
	std::unique_ptr<BaseCollider> coll(collider);
	this->collider_ = coll.get();
	CollisionManager::GetInstance()->AddCollider(std::move(coll));
	collider_->Update();
}

void Object3D::SetAlphaColor(float alpha)
{
	color_.w = alpha;
}

void Object3D::SetAttribute(unsigned short attribute)
{
	collider_->SetAttribute(attribute);
}

void Object3D::SetModel(IModel* model)
{
	model_ = model;
	animation_->SetModel(model);
}
