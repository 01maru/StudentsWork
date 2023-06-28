#pragma once
#include "AbstractSceneFactory.h"
#include "IScene.h"
#include "PostEffect.h"
#include <future>

#include "SplashSprite.h"
#include "ILoadingObj.h"
#include "FrameCounter.h"

#include "GaussBlur.h"
#include "GlayScale.h"

class SceneManager
{
private:
	FrameCounter sceneChangeCounter_;

	std::unique_ptr<IScene> scene_;
	std::unique_ptr<IScene> nextScene_;

	std::unique_ptr<AbstractSceneFactory> sceneFactry_;

	Vector4D screenColor_ = { 1.0f,1.0f,1.0f,1.0f };

#pragma region SplashScreen

	bool isSplashScreen_ = false;
	std::unique_ptr<SplashSprite> splashSprite_;

#pragma endregion

#pragma region Loading
	std::future<void> sceneInitInfo_;
	bool endLoading_ = false;
	bool sceneInitialized_ = true;

	std::unique_ptr<ILoadingObj> loadObj_;
#pragma endregion

#pragma region PostEffect
	std::unique_ptr<PostEffect> mainScene;
	std::unique_ptr<PostEffect> taskScene;
	std::unique_ptr<PostEffect> luminnce;

	std::unique_ptr<PostEffect> shadowEffect;
	std::unique_ptr<GaussBlur> luminnceBulr;
	std::unique_ptr<GaussBlur> shadowBulr;
	std::unique_ptr<GlayScale> glayscale;

	std::unique_ptr<PostEffect> dofscene;
	std::unique_ptr<GaussBlur> strongBulr;
	std::unique_ptr<GaussBlur> weakBulr;
#pragma endregion

private:	//	関数
	void ScreenColorUpdate();
	void SplashUpdate();
	void SceneFadeInUpdate();
	void SceneFadeOutUpdate();
	void SceneAsyncUpdate();
	void SceneUpdate();
	void ImguiUpdate();

	void FirstSceneInitialize();		//	SplashScreen用
	void SplashScreenInitialize();
	void SceneChange();

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

	void ChangeScreenColor(const Vector4D& color) { mainScene->SetColor(color); }
	void SetNextScene(const std::string& sceneName);

	Texture* GetShadowMap() { return shadowBulr->GetTexture(0); }
};

