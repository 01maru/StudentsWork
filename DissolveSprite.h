#pragma once
#include "Sprite.h"

namespace CBuff {
	struct CBuffDissolveData;
}

class DissolveSprite :public Sprite
{
private:
	bool dirtyFlagDissolve_ = true;
	float disolveValue_ = 0.0f;
	Vector4D dissolveColor_;

#pragma region CBuff
	ConstBuff cbDissolve_;
	CBuff::CBuffDissolveData* cbDissolveMat_ = nullptr;
#pragma endregion

public:
	void Initialize(Texture* texture = nullptr);
	void Update();
	void Draw(GPipeline* pipeline = nullptr) override;

	//	Getter
	float const GetDissolveValue() { return disolveValue_; }
	Vector4D& GetDissolveColor() { return dissolveColor_; }

	//	Setter
	void SetDissolveValue(float value);
};

