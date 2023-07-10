#pragma once
#include "IScene.h"
#include "Sprite.h"

class TitleScene :public IScene
{
private:
#pragma region Sprite

	std::unique_ptr<Sprite> backSprite_;

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

