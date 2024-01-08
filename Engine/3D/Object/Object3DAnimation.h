#pragma once
#include "ConstBuff.h"
#include <cstdint>
#include "IModel.h"

namespace CBuff {
	struct CBuffSkinData;
}

class Object3DAnimation
{
public:
	void Initialize();
	void MatUpdate();
	void SetGraphicsRootCBuffView(int32_t rootparaIndex);
private:
	CBuff::CBuffSkinData* cSkinMap_ = nullptr;
	ConstBuff skinData_;

	float animationTimer_ = 0.0f;
	int32_t animationIdx = -1;

	IModel* model_ = nullptr;

public:
	void SetAnimationIdx(int32_t idx) { animationIdx = idx; }
	void SetAnimatonTimer(float timer) { animationTimer_ = timer; }
	float GetAnimationTimer() const { return animationTimer_; }
	void SetModel(IModel* model);
};

