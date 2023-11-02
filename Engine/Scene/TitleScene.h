#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "FrameCounter.h"
#include "TitleUI.h"

#include "JSONLoader.h"

class TitleScene :public IScene
{
private:
	std::unique_ptr<JSONLoader> level_;
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<Object3D> skydome_;

#pragma region Sprite

	TitleUI uiData_;
	bool drawUI_ = true;

#pragma endregion

private:	//	関数
	void MatUpdate() override;
	
public:
	~TitleScene() override {};
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void FirstFrameUpdate() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override;
	void Draw() override;
};

