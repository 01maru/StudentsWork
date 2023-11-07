#pragma once
#include "Particle.h"

#pragma region 前置宣言

class ParticleEmitter;

#pragma endregion

class EmitterComponent
{
public:
	virtual ~EmitterComponent() = default;
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	virtual void Initialize(Particle* /*particle*/) {};
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() {};
	/**
	* @fn MatUpdate()
	* 座標更新処理関数
	*/
	virtual void MatUpdate() {};
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw() {};

protected:
	//	親Object
	ParticleEmitter* parent_ = nullptr;

public:
	/**
	* @fn SetParent(UIObject*)
	* parent_の設定用関数
	* @param parent parent_の変更後の値
	*/
	void SetParent(ParticleEmitter* parent);
};

