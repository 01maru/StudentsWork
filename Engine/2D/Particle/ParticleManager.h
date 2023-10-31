#pragma once
#include "IParticle.h"
#include <forward_list>
#include <memory>

/**
* @file ParticleManager.h
* @brief すべてのパーティクルをまとめて管理するファイル
*/

class GPipeline;

class ParticleManager
{
private:
	//	ぺらポリゴンのパーティクルリスト
	std::forward_list<std::unique_ptr<IParticle>> particles_;
	//	ぺらポリゴンの描画に使用するパイプライン配列
	std::vector<GPipeline*> pipelines_;
private:
	ParticleManager() {};
	~ParticleManager() {};

public:
	static ParticleManager* GetInstance();
	//	コピーコンストラクタ無効
	ParticleManager(const ParticleManager& obj) = delete;
	//	代入演算子無効
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

#pragma region ADD

	/**
	* @fn AddParticle(std::unique_ptr<IParticle>&)
	* パーティクル追加用関数
	* @param particle 追加するパーティクルの情報
	*/
	void AddParticle(std::unique_ptr<IParticle>& particle);

#pragma endregion
};

