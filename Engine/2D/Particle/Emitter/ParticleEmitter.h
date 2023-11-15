#pragma once
#include <cstdint>
#include "Vector3D.h"
#include "EmitterComponent.h"
#include <list>
#include <memory>

/**
* @file ParticleEmitter.h
* @brief パーティクルを発生させるためのクラスファイル
*/

class ParticleEmitter
{
public:
	/**
	* @fn Update()
	* 更新処理用関数
	*/
	void Update();

public:
	//	発生方向
	enum EmitterType
	{
		OmniType,
		Directional,
		Surface,
		Curve,
		Volume,
	};

private:
	//	オブジェクトパーティクルかどうか
	bool isObj_ = false;
	//	実行中か
	bool isActive_ = true;
	//	Emitterの座標
	Vector3D pos_;
	//	実行時間
	int32_t timer_ = 0;
	//	パーティクルの発生レート
	int32_t rate_ = 70;
	//	ブレンド設定
	Blend::BlendMord blendMord_ = Blend::ALPHA_BLEND;

	int32_t lifeTime_ = 60;
	//	発生させるパーティクルの情報
	std::list<std::unique_ptr<EmitterComponent>> components_;

	//　設定する項目(サイズ、回転、LifeTime、重力)

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

	Vector3D& GetPosition();

#pragma endregion

#pragma region Setter

	void SetIsObj(bool isObj);

	void SetRate(int32_t v);

	void SetPosition(const Vector3D& pos);

	void SetLifeTime(int32_t lifeTime);

	void SetBlendMord(Blend::BlendMord blendMord);

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
