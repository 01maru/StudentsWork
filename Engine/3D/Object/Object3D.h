#pragma once
#include "MyMath.h"
#include "CollisionInfo.h"

#include "ConstBuff.h"
#include "Object3DAnimation.h"
#include "Object3DShadow.h"

#pragma region 前置宣言

namespace CBuff {
	struct CBuffObj3DTransform;
	struct CBuffColorMaterial;
}

class BaseCollider;
class ICamera;
class IModel;
class GPipeline;

#pragma endregion

class Object3D
{
public:
	virtual ~Object3D();

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate();
	void DrawShadow();
	virtual void Draw();

private:

	ICamera* camera_ = nullptr;

	GPipeline* pPipeline_ = nullptr;

	//	影生成するか
	bool shadowing_ = false;
	//	影の影響を受けるか
	bool shadowReciev_ = false;

#pragma region CBuff
	
	CBuff::CBuffObj3DTransform* cTransformMap_ = nullptr;

	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;
	
#pragma endregion

	Object3D* parent_ = nullptr;
protected:
	MyMath::ObjMatrix mat_;
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	BaseCollider* collider_ = nullptr;
	ConstBuff transform_;
	ConstBuff colorMaterial_;

	IModel* model_ = nullptr;

	std::unique_ptr<Object3DAnimation> animation_;

	std::unique_ptr<Object3DShadow> shadow_;

protected:
	void DrawModel(int32_t& rootParaIdx);
private:
	void DrawShadowReciever(int32_t& nextIdx);

public:
	static std::unique_ptr<Object3D> Create(IModel* model_ = nullptr);

	virtual void OnCollision(CollisionInfo& info) { (void)info; }

#pragma region Getter

	BaseCollider* GetCollider() { return collider_; }
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

