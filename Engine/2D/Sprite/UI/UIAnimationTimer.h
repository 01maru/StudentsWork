#pragma once
#include "UIComponent.h"
#include "FrameCounter.h"

/**
* @file UIAnimationTimer.h
* @brief UIのアニメーションの時間管理用のファイル
*/

namespace MNE
{

	class UIAnimationTimer :public UIComponent, public FrameCounter
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
		//	カウント開始までの時間
		int32_t startCount_ = 0;

	public:
	#pragma region Getter

		/**
		* @fn GetStartCount()
		* カウント開始までの時間のGetter関数(カーソル用)
		* @return カウント開始までの時間
		*/
		int32_t GetStartCount();

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetStartCount(int32_t)
		* カウント開始時間設定用関数
		* @param startCount カウント開始までの時間
		*/
		void SetStartCount(int32_t startCount);

		void Reset() override;

	#pragma endregion
	};

}
