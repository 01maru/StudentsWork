#pragma once
#include "ConstBuff.h"

namespace MNE
{

	class Object3D;
	namespace CBuff {
		struct CBuffObj3DTransform;
	}

	class Object3DShadow
	{
	public:
		void Initialize();
		void MatUpdate();
		void Draw();

	private:
		MNE::ConstBuff shadowTransform_;
		MNE::CBuff::CBuffObj3DTransform* cShadowTransMap_ = nullptr;
		Object3D* parent_ = nullptr;

	public:
	#pragma region Setter

		/**
		* @fn SetParent(Object3D*)
		* parent_の設定用関数
		* @param parent parent_の変更後の値
		*/
		void SetParent(Object3D* parent);

	#pragma endregion
	};

}
