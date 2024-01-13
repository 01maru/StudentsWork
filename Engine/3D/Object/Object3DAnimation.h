#pragma once
#include "ConstBuff.h"
#include <cstdint>
#include "IModel.h"

namespace MNE
{

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
		MNE::CBuff::CBuffSkinData* cSkinMap_ = nullptr;
		MNE::ConstBuff skinData_;

		float animationTimer_ = 0.0f;
		int32_t animationIdx = -1;

		IModel* model_ = nullptr;

		bool isLoop_ = false;
		bool isEnd_ = false;

	public:
	#pragma region Getter

		float GetAnimationTimer() const { return animationTimer_; }

	#pragma endregion

	#pragma region Setter

		void SetAnimationIdx(int32_t idx) { animationIdx = idx; }
		void SetAnimatonTimer(float timer) { animationTimer_ = timer; }
		void SetModel(IModel* model);

	#pragma endregion
	};

}
