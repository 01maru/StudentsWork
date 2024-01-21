#pragma once
#include "ConstBuff.h"
#include <cstdint>
#include <string>
#include "IModel.h"

namespace MNE
{
#pragma region 前置宣言

	namespace CBuff {
		struct CBuffSkinData;
	}
	class IModel;

#pragma endregion


	class Object3DAnimation
	{
	public:
		void Initialize();
		void MatUpdate();
		void SetGraphicsRootCBuffView(int32_t rootparaIndex);

	private:
#pragma region ConstBuff

		MNE::CBuff::CBuffSkinData* cSkinMap_ = nullptr;
		MNE::ConstBuff skinData_;

#pragma endregion

		bool isLoop_ = true;
		IModel* model_ = nullptr;
		float animeTimer_ = 0.0f;
		std::string animeName_ = "";

		bool autoPlay_ = false;
		bool isEnd_ = false;

	public:
	#pragma region Getter

		float GetAnimationTimer() const;

	#pragma endregion

	#pragma region Setter

		void SetAutoPlay(bool autoPlay);
		void SetIsLoop(bool isLoop);
		void SetAnimeName(const std::string& name);
		void SetAnimeTimer(float timer);
		void SetModel(IModel* model);

	#pragma endregion
	};

}
