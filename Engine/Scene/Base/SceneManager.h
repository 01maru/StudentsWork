#pragma once
#include "AbstractSceneFactory.h"
#include "IScene.h"
#include <future>

#include "SplashScreenScene.h"
#include "LoadingScene.h"

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

		void Initialize();
		void Finalize();
		void Update();
		void DrawShadow();
		void DrawScene();
		void Draw();

	private:
		bool drawShadow_ = false;
		bool gameLoop_ = true;

		std::unique_ptr<IScene> scene_;
		std::unique_ptr<IScene> nextScene_;
		std::unique_ptr<AbstractSceneFactory> sceneFactry_;

		Sprite blackScreen_;

#pragma region SplashScreen

		bool isSplashScreen_ = false;
		std::unique_ptr<SplashScreenScene> splashScene_;

#pragma endregion

#pragma region Loading

		std::future<void> sceneInitInfo_;
		bool endLoading_ = false;
		LoadingScene loading_;

#pragma endregion

	private:	//	関数
		void SplashUpdate();
		void AllSceneUpdate();
		void SceneAsyncInitialize();
		void SceneAsyncUpdate();
		void SceneUpdate();
		void ImGuiUpdate();

		void DrawBackBuffer();

		void SceneInitialize();		//	SplashScreen用
		void FirstScreenInitialize();
		void SceneChange();

	public:
#pragma region Getter

		bool GetIsDrawShadow();
		bool GetGameLoop();

#pragma endregion

#pragma region Setter

		void GameLoopEnd();
		void ChangeScreenAlpha(float alpha);
		void SetNextScene(const std::string& sceneName);

#pragma endregion
	};

}
