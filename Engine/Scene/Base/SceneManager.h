#pragma once
#include "AbstractSceneFactory.h"
#include "IScene.h"
#include <future>

#include "SplashScreenScene.h"
#include "LoadingScene.h"

/**
* @file SceneManager.h
* @brief シーンの切り替えなどを管理するファイル
*/

namespace MNE
{

	class SceneManager
	{
	private:
		SceneManager() {};
		~SceneManager() {};

	public:
		static SceneManager* GetInstance();
		SceneManager(const SceneManager& obj) = delete;
		SceneManager& operator=(const SceneManager& obj) = delete;

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize();
		/**
		* @fn Finalize()
		* 終了時に呼び出す関数
		*/
		void Finalize();
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update();
		/**
		* @fn DrawShadow()
		* シャドウマップ描画処理関数
		*/
		void DrawShadow();
		/**
		* @fn DrawScene()
		* シーンの描画処理関数
		*/
		void DrawScene();
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		void Draw();

	private:
		//	デバッグ中か
		bool debugging_ = FALSE;

		bool drawShadow_ = false;
		bool gameLoop_ = true;

		std::unique_ptr<IScene> scene_;
		std::unique_ptr<IScene> nextScene_;
		std::unique_ptr<AbstractSceneFactory> sceneFactory_;

#pragma region SplashScreen

		bool isSplashScreen_ = false;
		std::unique_ptr<SplashScreenScene> splashScene_;

#pragma endregion

#pragma region Loading

		std::future<void> sceneInitInfo_;
		//	非同期ロード中か
		bool endLoading_ = false;
		LoadingScene loading_;

#pragma endregion

	private:	//	関数
		void SceneInitialize();		//	SplashScreen用
		void FirstScreenInitialize();
		void SceneAsyncInitialize();
		void SplashUpdate();
		void AllSceneUpdate();
		void SceneAsyncUpdate();
		void SceneUpdate();

		/**
		* @fn DrawBackBuffer()
		* ポストエフェクトで処理した画像を最終的にバックバッファに描画する処理関数
		*/
		void DrawBackBuffer();

		/**
		* @fn SceneManagerImGuiUpdate()
		* シーンマネージャーのImGui更新処理関数
		*/
		void SceneManagerImGuiUpdate();
		/**
		* @fn ImGuiUpdate()
		* ImGui更新処理関数
		*/
		void ImGuiUpdate();

		/**
		* @fn SceneChange()
		* シーンの切り替え処理関数
		*/
		void SceneChange();

	public:
#pragma region Getter

		bool GetIsDrawShadow();
		bool GetGameLoop();

#pragma endregion

#pragma region Setter

		void GameLoopEnd();
		void SetNextScene(const std::string& sceneName);

#pragma endregion
	};

}
