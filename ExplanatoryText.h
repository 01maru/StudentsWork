#pragma once
#include "UIData.h"
#include "Sprite.h"

namespace MNE
{
	class Texture;
}

class ExplanatoryText :public MNE::UIData
{
public:
	void Initialize();
	void LoadResources(const std::string& uiFileName, const std::string& cursorSpriteName);
	void Update(bool prevUsePad);
	void MatUpdate();
	void Draw();

private:
	bool draw_ = false;

	//	マウスカーソルスプライト
	MNE::Sprite cursor_;
	//	マウスカーソル画像
	MNE::Texture* cursorTex_ = nullptr;

	bool dirtyFlag_ = false;
	bool playAnimation_ = false;

	std::string nextTag_ = "";

public:
	void SetDrawFlag(bool flag);
	void SetNextAnimetionTag(const std::string& tagName, bool playAnimation, bool startingAnimation);
};

