#pragma once
#include "MyMath.h"
#include "CollisionInfo.h"

#include "ConstBuff.h"
#include "Object3DAnimation.h"
#include "Object3DShadow.h"

#pragma region 前置宣言


class BaseCollider;

namespace MNE
{

	namespace CBuff {
		struct CBuffObj3DTransform;
		struct CBuffColorMaterial;
	}
	class GPipeline;
	class ICamera;
	class IModel;

}

#pragma endregion

namespace MNE
{

	class Object3D
	{
	public:
		virtual ~Object3D();

		virtual void Initialize();
		virtual void ColliderUpdate();
		void MatUpdate(bool calcRot = true);
		void DrawShadow();
		virtual void Draw();

	private:

		MNE::ICamera* camera_ = nullptr;

		MNE::GPipeline* pPipeline_ = nullptr;

		//	影生成するか
		bool shadowing_ = false;
		//	影の影響を受けるか
		bool shadowReciev_ = false;

	#pragma region CBuff
	
		MNE::CBuff::CBuffObj3DTransform* cTransformMap_ = nullptr;

		MNE::CBuff::CBuffColorMaterial* cColorMap_ = nullptr;

		MNE::ConstBuff transform_;
		MNE::ConstBuff colorMaterial_;

		std::unique_ptr<Object3DAnimation> animation_;

		std::unique_ptr<Object3DShadow> shadow_;

	#pragma endregion

		Object3D* parent_ = nullptr;
	protected:
		MyMath::ObjMatrix mat_;
		Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
		BaseCollider* collider_ = nullptr;

		IModel* model_ = nullptr;

	protected:
		void DrawModel(int32_t& rootParaIdx);
	private:
		void DrawShadowReciever(int32_t& nextIdx);

	public:
		virtual void OnCollision(CollisionInfo& info) { (void)info; }

	#pragma region Getter

		static std::unique_ptr<Object3D> Create(IModel* model_ = nullptr);

		BaseCollider* GetCollider() { return collider_; }
		const Vector4D& GetColor() { return color_; }
		Vector3D& GetPosition() { return mat_.trans_; }
		const Vector3D& GetScale() { return mat_.scale_; }
		const Vector3D& GetRotation() { return mat_.angle_; }
		inline IModel* GetModel() { return model_; }
		const Matrix& GetMatWorld() { return mat_.matWorld_; }
		Object3DAnimation* GetAnimation() { return animation_.get(); }

	#pragma endregion

	#pragma region Setter

		void SetModel(IModel* model);
		void SetCollider(BaseCollider* collider);
		void SetAttribute(unsigned short attribute);
		void SetColor(const Vector4D& color) { color_ = color; }
		void SetColor(const Vector3D& color) { color_ = Vector4D(color, color_.w); }
		void SetAlphaColor(float alpha);
		void SetPosition(const Vector3D& pos) { mat_.trans_ = pos; }
		void SetScale(const Vector3D& scale) { mat_.scale_ = scale; }
		void SetRotation(const Vector3D& rot) { mat_.angle_ = rot; }
		void SetCamera(MNE::ICamera* camera) { camera_ = camera; }

	#pragma endregion
	};

}
