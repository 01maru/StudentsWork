#pragma once
#include "UIComponent.h"

/**
* @file UIFadeAnimation.h
* @brief UIButtonの透過アニメーション用ファイル
*/

namespace MNE
{

#pragma region 前置宣言

	class UISprite;
	class UIAnimationTimer;

#pragma endregion

	class UIFadeAnimation :public UIComponent
	{
	public:
		/**
		* @fn Initialize()
		* 初期化関数
		*/
		void Initialize() override;
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update() override;

	private:
		//	透過するスプライト
		UISprite* sprite_ = nullptr;
		//	全体のTimer
		UIAnimationTimer* timer_ = nullptr;
		//	初期値
		float start_;
		//	終了値
		float end_;
	
	public:
	#pragma region Getter

		/**
		* @fn GetStart()
		* 透過初期値のGetter関数
		* @return 透過初期値
		*/
		float GetStart();
		/**
		* @fn GetEnd()
		* 透過終了値のGetter関数
		* @return 透過終了値
		*/
		float GetEnd();

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetStart(float)
		* 透過初期値設定用関数
		* @param start 変更後の透過初期値
		*/
		void SetStart(float start);
		/**
		* @fn SetEnd(float)
		* 透過終了値設定用関数
		* @param end 変更後の透過終了値
		*/
		void SetEnd(float end);

		void Reset() override;

	#pragma endregion
	};

}
