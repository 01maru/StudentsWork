#pragma once
#include <cstdint>
#include "Vector3D.h"
#include "EmitterComponent.h"
#include <list>
#include <memory>
#include "FrameCounter.h"
#include "EmitterType.h"

/**
* @file ParticleEmitter.h
* @brief パーティクルを発生させるためのクラスファイル
*/

namespace MNE
{

	class ParticleEmitter
	{
	public:
		/**
		* @fn Update()
		* 更新処理用関数
		*/
		void Update();

	private:
		bool isDead_ = false;
		std::unique_ptr<FrameCounter> deadTimer_;
		std::unique_ptr<EmitterType> shapeType_;
		//	オブジェクトパーティクルかどうか
		bool isObj_ = false;
		//	実行中か
		bool isActive_ = true;
		//	Emitterの座標
		MyMath::Vector3D pos_;
		//	生成レート
		FrameCounter timer_;
		//	ブレンド設定
		MNE::Blend::BlendMord blendMord_ = MNE::Blend::ALPHA_BLEND;

		int32_t lifeTime_ = 60;
		//	発生させるパーティクルの情報
		std::list<std::unique_ptr<EmitterComponent>> components_;

	private:
		void AddParticle();

	public:
		/**
		* @fn AddComponent()
		* コンポーネント追加用関数
		*/
		template<class T>
		T* AddComponent();
		/**
		* @fn DeleteComponent()
		* コンポーネント削除用関数
		*/
		template<class T>
		void DeleteComponent();

	#pragma region Getter

		/**
		* @fn GetComponent()
		* コンポーネント取得用関数
		*/
		template<class T>
		T* GetComponent();

		bool GetIsObj();

		MyMath::Vector3D& GetPosition();

		EmitterType* GetEmitterType() { return shapeType_.get(); }

		bool GetIsDead() { return isDead_; }

	#pragma endregion

	#pragma region Setter

		void SetIsObj(bool isObj);

		void SetRate(int32_t v);

		void SetPosition(const MyMath::Vector3D& pos);

		void SetLifeTime(int32_t lifeTime);

		void SetBlendMord(MNE::Blend::BlendMord blendMord);

		void SetEmitterType(std::unique_ptr<EmitterType>& type);

		void SetIsDead(bool isDead) { isDead_ = isDead; }

		void SetDeadTimer(std::unique_ptr<FrameCounter>& timer);

	#pragma endregion
	};

	template<class T>
	inline T* ParticleEmitter::AddComponent()
	{
		std::unique_ptr<T> component = std::make_unique<T>();
		component->SetParent(this);
		//component->Initialize();
		components_.push_back(std::move(component));

		return dynamic_cast<T*>(components_.back().get());
	}

	template<class T>
	inline void ParticleEmitter::DeleteComponent()
	{
	}

	template<class T>
	inline T* ParticleEmitter::GetComponent()
	{
		for (auto itr = components_.begin(); itr != components_.end(); ++itr)
		{
			T* compo = dynamic_cast<T*>(itr->get());
			if (compo != nullptr)	return compo;
		}
		return nullptr;
	}

}
