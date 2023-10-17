#pragma once
#include <forward_list>
#include "ParticleStruct.h"

/**
* @file ParticleManager.h
* @brief すべてのパーティクルをまとめて管理するファイル
*/

class Texture;

class ParticleManager
{
private:
	std::forward_list<MoveParticle> particles_;

#pragma region Texture
	Texture* particleTex_ = nullptr;
#pragma endregion

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

#pragma region ADD

	void AddMoveParticle(const Vector3D& pos, const Vector3D& spd, int time, float scale = 3.0f);

#pragma endregion
};

