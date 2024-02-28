#pragma once
#include "EmitterComponent.h"

namespace MNE
{

	class EmitterAccComponent :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(Particle* particle) override;

	private:
		float spd_ = 0.05f;
		float acc_ = 0.01f;
		MyMath::Vector3D accVec_;

	public:
		void SetSpd(float spd);
		void SetAcc(float acc);
		void SetAccVec(const MyMath::Vector3D& v);
	};

}
