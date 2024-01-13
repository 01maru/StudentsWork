#pragma once
#include "UIComponent.h"
#include "Vector2D.h"

/**
* @file UIScaling.h
* @brief UISpriteを拡縮させるためのファイル
*/

namespace MNE
{

#pragma region 前置宣言

	class UISprite;
	class UIAnimationTimer;

#pragma endregion

	class UIScaling :public UIComponent
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
		Vector2D startSize_;
		Vector2D endSize_;

		UISprite* pSprites_ = nullptr;
		//	全体のtimer
		UIAnimationTimer* pTimer_ = nullptr;

	public:
	#pragma region Getter

		/**
		* @fn GetStartSize()
		* スプライトの開始時サイズのGetter関数
		* @return スプライトの開始時サイズ
		*/
		Vector2D& GetStartSize();
		/**
		* @fn GetEndSize()
		* スプライトの終了時サイズのGetter関数
		* @return スプライトの終了時サイズ
		*/
		Vector2D& GetEndSize();

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetStartSize(const Vector2D&)
		* スプライトの開始時サイズ設定用関数
		* @param size startSize_の変更値
		*/
		void SetStartSize(const Vector2D& size);
		/**
		* @fn SetEndSize(const Vector2D&)
		* スプライトの終了時サイズ設定用関数
		* @param size endSize_の変更値
		*/
		void SetEndSize(const Vector2D& size);

	#pragma endregion
	};

}
