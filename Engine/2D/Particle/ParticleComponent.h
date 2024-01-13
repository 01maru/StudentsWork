#pragma once
/**
* @file UIComponent.h
* @brief コンポーネント指向で作成するUI用継承クラスファイル
*/

namespace MNE
{

	#pragma region 前置宣言

	class Particle;

	#pragma endregion

	class ParticleComponent
	{
	public:
		virtual ~ParticleComponent() = default;
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		virtual void Initialize() {};
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
		Particle* parent_ = nullptr;

	public:
	#pragma region Setter

		/**
		* @fn SetParent(UIObject*)
		* parent_の設定用関数
		* @param parent parent_の変更後の値
		*/
		void SetParent(Particle* parent);

	#pragma endregion
	};

}
