#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "UIDrawer.h"
#include "OptionScene.h"
#include "FrameCounter.h"

#include "JSONLoader.h"

class TitleScene :public IScene
{
private:
	enum Mord
	{
		GameStart,
		Option,
		GameEnd,
	};

private:
	std::unique_ptr<JSONLoader> level_;
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<Object3D> skydome_;
	std::unique_ptr<Sprite> selectCursor_;
	FrameCounter selectCounter_;

	uint16_t selectMord_ = 0;

	std::unique_ptr<OptionScene> optionScene_;

#pragma region Sprite

	std::unique_ptr<UIDrawer> uiDrawer_;
	bool drawUI_ = true;

#pragma endregion

private:	//	関数
	void MatUpdate() override;
	
public:
	~TitleScene() override {};
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override;
	void Draw() override;
};

