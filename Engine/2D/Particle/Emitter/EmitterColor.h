#pragma once
#include "EmitterComponent.h"
#include "Vector3D.h"

namespace MNE
{

	class EmitterColor :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;

	private:
		MyMath::Vector3D color_;

	public:
		void SetColor(const MyMath::Vector3D& color);
	};

}
