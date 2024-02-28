#pragma once
#include "EmitterType.h"

/**
* @file EmitterCircumType.h
* @brief 円周上にパーティクルを生成する型のファイル
*/

namespace MNE
{

	class EmitterCircumType :public EmitterType
	{
	public:
		std::vector<std::unique_ptr<MNE::Particle>> Update() override;

	private:
		//	半径
		float radius_ = 0.1f;
	};

}
