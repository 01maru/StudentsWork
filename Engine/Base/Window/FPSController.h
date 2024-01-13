#pragma once
#include <chrono>
#include <list>

/**
* @file FPSController.h
* @brief FPS制御用
*/

namespace MNE
{

	class FPSController
	{
	private:
		//	Listのサイズ
		static const size_t sFPSLIST_MAX_SIZE = 60;

		//	sFPSLIST_MAX_SIZEフレーム分のfps保存用リスト
		std::list<float> fpsList_;
		//	fpsList_の合計値
		float totalFps_ = 0.0f;
		//	fpsList_の平均値(表示用)
		float fpsValue_ = 0.0f;

		std::chrono::steady_clock::time_point reference_;

	private:
		void CalcFps();

		FPSController() {};
		~FPSController() {};
	public:
		static FPSController* GetInstance();
		FPSController(const FPSController& obj) = delete;
		FPSController& operator=(const FPSController& obj) = delete;

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize();

		/**
		* @fn Update()
		* 更新処理をまとめた関数
		*/
		void Update();

#pragma region Getter

		/**
		* @fn GetFPS()
		* FPS値を返す関数
		* @return 現在のFPSの値
		*/
		float GetFPS() { return fpsValue_; }

#pragma endregion
	};

}
