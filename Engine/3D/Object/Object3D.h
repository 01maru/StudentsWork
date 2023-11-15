#pragma once
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
class GPipeline;

class Object3D
{
private:

	ICamera* camera_ = nullptr;

	GPipeline* pPipeline_ = nullptr;

	//	影生成するか
	bool shadowing_ = false;
	//	影の影響を受けるか
	bool shadowReciev_ = false;

#pragma region CBuff
	
	CBuff::CBuffObj3DTransform* cTransformMap_ = nullptr;

	ConstBuff shadowTransform_;
	CBuff::CBuffObj3DTransform* cShadowTransMap_ = nullptr;

	CBuff::CBuffLightMaterial* cLightMap_ = nullptr;

	CBuff::CBuffSkinData* cSkinMap_ = nullptr;

	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;
	
#pragma endregion

	Object3D* parent_ = nullptr;
	float animationTimer_ = 0.0f;
protected:
	MyMath::ObjMatrix mat_;
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	BaseCollider* collider_ = nullptr;
	ConstBuff transform_;
	ConstBuff lightMaterial_;
	ConstBuff skinData_;
	ConstBuff colorMaterial_;

	IModel* model_ = nullptr;
	
private:
	void DrawShadow(bool drawShadow);
	void DrawShadowReciever(bool drawShadow);
	void DrawShadowUnReciever(bool drawShadow);

public:
	Object3D() = default;
	virtual ~Object3D();

	static Object3D* Create(IModel* model_ = nullptr);

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate(int32_t animationIdx = -1);
	virtual void Draw(bool drawShadow);

	virtual void Draw();

	virtual void OnCollision(CollisionInfo& info) { (void)info; }

	void SetAnimatonTimer(float timer) { animationTimer_ = timer; }
	float GetAnimationTimer() const { return animationTimer_; }

	BaseCollider* GetCollider() { return collider_; }
	
#pragma region Getter

	const Vector4D& GetColor() { return color_; }
	const Vector3D& GetPosition() { return mat_.trans_; }
	const Vector3D& GetScale() { return mat_.scale_; }
	const Vector3D& GetRotation() { return mat_.angle_; }
	inline IModel* GetModel() { return model_; }
	const Matrix& GetMatWorld() { return mat_.matWorld_; }

#pragma endregion

#pragma region Setter

	void SetModel(IModel* model);
	void SetCollider(BaseCollider* collider);
	void SetAttribute(unsigned short attribute);
	void SetColor(const Vector4D& color) { color_ = color; }
	void SetColor(const Vector3D& color) { color_ = Vector4D(color, color_.w); }
	void SetPosition(const Vector3D& pos) { mat_.trans_ = pos; }
	void SetScale(const Vector3D& scale) { mat_.scale_ = scale; }
	void SetRotation(const Vector3D& rot) { mat_.angle_ = rot; }
	void SetCamera(ICamera* camera) { camera_ = camera; }

#pragma endregion
};

