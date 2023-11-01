#pragma once
#include <cstdint>

class ParticleEmitter
{
public:
	enum EmitterType
	{
		OmniType,
		Directional,
		Surface,
		Curve,
		Volume,
	};
private:
	int32_t rate_;	//	パーティクルの出現レート
};

