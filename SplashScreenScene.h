#pragma once
#include "IScene.h"
#include "SplashSprite.h"
#include "FrameCounter.h"

class SplashScreenScene :public IScene
{
private:
	std::unique_ptr<FrameCounter> count_;

	std::unique_ptr<SplashSprite> engineSprite_;
	std::unique_ptr<Sprite> backSprite_;

private:	//	関数
	void MatUpdate() override {};

public:
	~SplashScreenScene() override {};
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override {};
	void Draw() override;

	bool EndScene() { return !count_->GetIsActive(); }
};

