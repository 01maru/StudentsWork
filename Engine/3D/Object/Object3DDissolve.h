#pragma once
#include "Object3D.h"

namespace MNE
{

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

		MNE::ConstBuff dissolve_;
		MNE::CBuff::CBuffDissolveData* cDissolveMap_ = nullptr;

	#pragma endregion

		float dissolveValue_ = 1.0f;
		MyMath::Vector4D dissolveColor_;

	public:
	};

}
