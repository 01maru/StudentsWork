#pragma once
#include "Sprite.h"

class CrossHair
{
public:
	void Update(const Vector3D& startPos);
	void ImGuiUpdate();
	void Draw();

private:
	Sprite sprite_;
	Vector3D dir_;
	const float MAX_DISTANCE = 30.0f;

public:
	void SetSprite(const Sprite& sprite) { sprite_ = sprite; }
	Vector3D GetDir() { return dir_; }
};

