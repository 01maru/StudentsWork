#pragma once
#include "Object3D.h"

namespace CBuff {
	struct CBuffDissolveData;
}

class Object3DDissolve :public Object3D
{
public:
	void Initialize();
	void MatUpdate();
	void Draw() override;

private:
#pragma region CBuff

	ConstBuff dissolve_;
	CBuff::CBuffDissolveData* cDissolveMap_ = nullptr;

#pragma endregion

	float dissolveValue_ = 1.0f;
	Vector4D dissolveColor_;

public:
};

