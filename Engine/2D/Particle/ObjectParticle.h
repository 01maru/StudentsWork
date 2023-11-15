#pragma once
#include "ParticleComponent.h"
#include "Object3D.h"

class ObjectParticle :public ParticleComponent, public Object3D
{
public:
	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize() override;
	/**
	* @fn MatUpdate()
	* 座標更新処理関数
	*/
	void MatUpdate() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

private:

public:
};

