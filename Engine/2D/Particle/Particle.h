#pragma once
#include "BlendMord.h"
#include "ParticleComponent.h"
#include "FrameCounter.h"
#include "Vector3D.h"
#include <list>
#include <memory>

/**
* @file Particle.h
* @brief パーティクル用のコンポーネントをまとめて管理するためのクラスファイル
*/

namespace MNE
{

	class Particle
	{
	public:
		/**
		* @fn Update()
		* 更新処理用関数
		*/
		void Update();
		/**
		* @fn MatUpdate()
		* 行列更新用関数
		*/
		void MatUpdate();
		/**
		* @fn Draw()
		* 描画処理用関数
		*/
		void Draw();

	protected:
		//	パーティクル終了フラグ
		bool isEnd_ = false;
		//	ブレンド設定
		MNE::Blend::BlendMode blendMode_ = MNE::Blend::ALPHA_BLEND;
		//	消滅までの時間
		FrameCounter lifeTimer_;
		//	パーティクルの移動方向
		MyMath::Vector3D dir_;
	
		//	コンポーネントリスト
		std::list<std::unique_ptr<ParticleComponent>> components_;

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
		/**
		* @fn GetBlendMode()
		* blendMode_のGetter関数
		* @return blendMode_
		*/
		Blend::BlendMode GetBlendMode();

		bool GetIsEnd();

		float GetTimerPerTime();

		MyMath::Vector3D GetDir();

	#pragma endregion

		void SetDir(const MyMath::Vector3D& dir);
		void SetBlendMode(Blend::BlendMode blendMode);
		void SetLifeTime(int32_t time);
		void StartTimer();
	};

	template<class T>
	inline T* Particle::AddComponent()
	{
		std::unique_ptr<T> component = std::make_unique<T>();
		component->SetParent(this);
		component->Initialize();
		components_.push_back(std::move(component));

		return dynamic_cast<T*>(components_.back().get());
	}

	template<class T>
	inline T* Particle::GetComponent()
	{
		for (auto itr = components_.begin(); itr != components_.end(); ++itr)
		{
			T* compo = dynamic_cast<T*>(itr->get());
			if (compo != nullptr)	return compo;
		}
		return nullptr;
	}

	template<class T>
	void Particle::DeleteComponent()
	{
	}

}
