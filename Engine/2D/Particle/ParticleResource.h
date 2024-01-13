#pragma once
#include "EmitterComponent.h"
#include <string>

/**
* @file ParticleResource.h
* @brief リソースを追加するためのパーティクルのコンポーネントクラスファイル
*/

namespace MNE
{

	class ParticleResource :public EmitterComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize(MNE::Particle* particle) override;

	private:
		//	リソースの名前
		std::string name_;

	public:
	#pragma region Setter

		/**
		* @fn SetResourceName(const std::string&)
		* パーティクルに設定するリソースの名前を変更するための関数
		* @param name パーティクルに設定するリソースの変更名
		*/
		void SetResourceName(const std::string& name);

	#pragma endregion
	};

}
