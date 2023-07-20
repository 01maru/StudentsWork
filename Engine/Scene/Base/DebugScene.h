#pragma once
#include "IScene.h"
#include "Object3D.h"
#include <memory>

class DebugScene :public IScene
{
private:
	std::unique_ptr<Object3D> fbx_;
	std::unique_ptr<Object3D> ground_;
	int32_t index = -1;
	int32_t timer = 0;
private:	//	関数
	void MatUpdate() override;

public:
	~DebugScene() override {};
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override;
	void Draw() override;
};

