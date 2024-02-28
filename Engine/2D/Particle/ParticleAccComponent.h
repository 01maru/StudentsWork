#pragma once
#include "ParticleComponent.h"
#include "Vector3D.h"

namespace MNE
{

	class ParticleAccComponent :public ParticleComponent
	{
	public:
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update() override;

	private:
		MyMath::Vector3D spd_;
		MyMath::Vector3D acc_;
		bool isObj_ = false;

	public:
#pragma region Getter

#pragma endregion

#pragma region Setter

		void SetSpd(const MyMath::Vector3D& spd);
		void SetAcc(const MyMath::Vector3D& acc);
		void SetIsObj(bool isObj);

#pragma endregion
	};

}

