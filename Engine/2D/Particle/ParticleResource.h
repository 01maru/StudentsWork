#pragma once
#include "EmitterComponent.h"
#include <string>

class ParticleResource :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;

private:
	std::string name_;

public:
	void SetResourceName(const std::string& name);
};

