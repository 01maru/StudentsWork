#pragma once
#include "Object3D.h"

/**
* @file Bonfire.h
* @brief 焚火の表示やエフェクトを管理するファイル
*/

#pragma region 前置宣言
namespace MNE
{
	class ParticleEmitter;
}
#pragma endregion

class Bonfire :public MNE::Object3D
{
public:
	/**
	* @fn Start()
	* エフェクト開始処理関数
	*/
	void Start();
	/**
	* @fn Stop()
	* エフェクト終了処理関数
	*/
	void Stop();

private:
	//	煙エフェクトのエミッター
	MNE::ParticleEmitter* smoke_ = nullptr;
	//	炎エフェクトのエミッター
	MNE::ParticleEmitter* fire_ = nullptr;
};

