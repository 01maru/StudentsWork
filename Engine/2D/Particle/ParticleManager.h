#pragma once
#include "Particle.h"
#include "ParticleEmitter.h"
#include <forward_list>
#include <memory>
#include "GPipeline.h"

/**
* @file ParticleManager.h
* @brief すべてのパーティクルをまとめて管理するファイル
*/

class ParticleManager
{
private:
	ParticleManager() {};
	~ParticleManager() {};

public:
	static ParticleManager* GetInstance();
	ParticleManager(const ParticleManager& obj) = delete;
	ParticleManager& operator=(const ParticleManager& obj) = delete;

	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize();
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
	* @fn ImGuiUpdate()
	* ImGui更新用関数
	*/
	void ImGuiUpdate();
	/**
	* @fn Draw()
	* 描画処理用関数
	*/
	void Draw();

private:
	//	エミッターリスト
	std::forward_list <std::unique_ptr<ParticleEmitter>> emitters_;
	//	ぺらポリゴンのパーティクルリスト
	std::forward_list<std::unique_ptr<Particle>> particles_;
	//	ぺらポリゴンの描画に使用するパイプライン配列
	std::vector<GPipeline*> pipelines_;

	//	モデルを使ったパーティクルのリスト
	std::forward_list<std::unique_ptr<Particle>> objParticles_;

public:
#pragma region ADD

	/**
	* @fn AddParticle(const Particle&)
	* パーティクル追加用関数
	* @param particle 追加するパーティクルの情報
	*/
	void AddParticle(std::unique_ptr<Particle>& particle, bool isObj);

#pragma endregion
};

