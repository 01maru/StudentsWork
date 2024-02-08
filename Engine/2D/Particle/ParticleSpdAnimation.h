#pragma once
#include "ParticleComponent.h"
#include "Vector3D.h"

namespace MNE
{

	class ParticleSpdAnimation :public MNE::ParticleComponent
	{
	public:
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update() override;

	private:
		MyMath::Vector3D spd_;
		bool isObj_ = false;

	public:
	#pragma region Getter

	#pragma endregion

	#pragma region Setter

		void SetSpd(const MyMath::Vector3D& spd);
		void SetIsObj(bool isObj);

	#pragma endregion
	};

}
