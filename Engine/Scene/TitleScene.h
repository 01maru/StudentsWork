#pragma once
#include "IScene.h"
#include "TitleUI.h"

#include "JSONLoader.h"

#include "Bonfire.h"

class TitleScene :public MNE::IScene
{
private:
	std::unique_ptr<JSONLoader> level_;
	std::unique_ptr<MNE::Object3D> ground_;
	std::unique_ptr<MNE::Object3D> skydome_;
	std::unique_ptr<Bonfire> bonfire_;

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
	void Draw() override;
};

