#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "UIData.h"

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
#pragma region Sprite

	std::unique_ptr<Sprite> backSprite_;
	std::unique_ptr<UIData> ui_;
	bool drawUI_ = true;
	
	uint16_t selectMord_ = 0;

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

