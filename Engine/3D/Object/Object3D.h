﻿#pragma once
#define NOMINMAX
#include "GPipeline.h"
#include "MyMath.h"
#include "CollisionInfo.h"

#include "ConstBuff.h"

namespace CBuff {
	struct CBuffLightMaterial;
	struct CBuffObj3DTransform;
	struct CBuffSkinData;
	struct CBuffColorMaterial;
}

class BaseCollider;
class ICamera;
class IModel;

class Object3D
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static GPipeline* sPipeline;
	ICamera* camera_ = nullptr;

#pragma region CBuff
	
	ConstBuff transform_;
	CBuff::CBuffObj3DTransform* cTransformMap_ = nullptr;

	ConstBuff shadowTransform_;
	CBuff::CBuffObj3DTransform* cShadowTransMap_ = nullptr;

	ConstBuff lightMaterial_;
	CBuff::CBuffLightMaterial* cLightMap_ = nullptr;

	ConstBuff skinData_;
	CBuff::CBuffSkinData* cSkinMap_ = nullptr;

	ConstBuff colorMaterial_;
	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;

#pragma endregion

	Object3D* parent_ = nullptr;
	IModel* model_ = nullptr;
	float animationTimer_ = 0.0f;
protected:
	MyMath::ObjMatrix mat_;
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	BaseCollider* collider = nullptr;
	
public:
	Object3D() = default;
	virtual ~Object3D();

	static Object3D* Create(IModel* model_ = nullptr);

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate();
	void PlayAnimation();
	virtual void DrawShadow();
	void DrawShadowReciever();
	virtual void Draw();
	void DrawSilhouette();

	static void SetPipeline(GPipeline* pipeline);
	void SetModel(IModel* model);

	void SetCollider(BaseCollider* collider_);
	BaseCollider* GetCollider() { return collider; }
	void SetAttribute(unsigned short attribute);

	void SetColor(const Vector4D& color) { color_ = color; }
	void SetColor(const Vector3D& color) { color_ = Vector4D(color, color_.w); }
	const Vector4D& GetColor() { return color_; }

	void SetAnimatonTimer(float timer) { animationTimer_ = timer; }
	const float GetAnimationTimer() { return animationTimer_; }

	void SetPosition(const Vector3D& pos) { mat_.trans_ = pos; }
	const Vector3D& GetPosition() { return mat_.trans_; }

	void SetScale(const Vector3D& scale) { mat_.scale_ = scale; }
	const Vector3D& GetScale() { return mat_.scale_; }

	void SetRotation(const Vector3D& rot) { mat_.angle_ = rot; }
	const Vector3D& GetRotation() { return mat_.angle_; }

	inline IModel* GetModel() { return model_; }
	const Matrix& GetMatWorld() { return mat_.matWorld_; }

	void SetCamera(ICamera* camera) { camera_ = camera; }

	virtual void OnCollision(const CollisionInfo& info) { (void)info; }
};

