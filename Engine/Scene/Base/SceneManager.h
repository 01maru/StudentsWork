#pragma once
#include "AbstractSceneFactory.h"
#include "IScene.h"
#include "PostEffect.h"
#include <future>
#include "GaussBlur.h"
#include "GlayScale.h"
#include "SplashScreenScene.h"

#include "LoadingScene.h"

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

#pragma region PostEffect
	std::unique_ptr<PostEffect> mainScene;
	std::unique_ptr<PostEffect> luminnce;

	std::unique_ptr<PostEffect> shadowEffect;
	std::unique_ptr<GaussBlur> luminnceBulr;
	std::unique_ptr<GaussBlur> shadowBulr;
	std::unique_ptr<GlayScale> glayscale;

	std::unique_ptr<GaussBlur> strongBulr;
	std::unique_ptr<GaussBlur> weakBulr;
#pragma endregion

private:	//	関数
	void SplashUpdate();
	void AllSceneUpdate();
	void SceneAsyncInitialize();
	void SceneAsyncUpdate();
	void SceneUpdate();
	void ImguiUpdate();

	void SceneInitialize();		//	SplashScreen用
	void FirstScreenInitialize();
	void SceneChange();

public:
#pragma region Getter

	bool GetIsDrawShadow() { return drawShadow_; }
	bool GetGameLoop() { return gameLoop_; }
	Texture* GetShadowMap() { return shadowEffect->GetTexture(0); }

#pragma endregion

#pragma region Setter

	void GameLoopEnd() { gameLoop_ = false; }
	void ChangeScreenAlpha(float alpha);
	void SetNextScene(const std::string& sceneName);

#pragma endregion
};

