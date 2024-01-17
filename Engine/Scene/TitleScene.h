#pragma once
#include "IScene.h"
#include "TitleUI.h"
#include "Bonfire.h"

class TitleScene :public MNE::IScene
{
public:
	~TitleScene() override {};
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void FirstFrameUpdate() override;
	void Update() override;
	void ImguiUpdate() override;
	void Draw() override;

private:	//	関数
	void MatUpdate() override;

private:
	std::vector<std::unique_ptr<MNE::Object3D>> objs_;

	std::unique_ptr<Bonfire> bonfire_;

#pragma region Sprite

	TitleUI uiData_;
	bool drawUI_ = true;

#pragma endregion
};

