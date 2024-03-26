#pragma once
#include "GaussBlur.h"
#include <list>

namespace MNE
{

	class PostEffectManager
	{
	private:
		PostEffectManager() {};
		~PostEffectManager() {};

	public:
		static PostEffectManager* GetInstance();
		PostEffectManager(const PostEffectManager& obj) = delete;
		PostEffectManager& operator=(const PostEffectManager& obj) = delete;
	
		void Update();
		void ImGuiUpdate();
		void DrawBackBuffer();

	private:
		std::list<std::unique_ptr<IPostEffect>> postEffects_;
		std::list<std::unique_ptr<GaussBlur>> gaussBlurs_;
		IPostEffect* backBuffer_ = nullptr;
		int32_t backBufferMode_ = 0;

	public:
		IPostEffect* AddPostEffect(std::unique_ptr<IPostEffect>& postEffect, const std::list<std::unique_ptr<IPostEffect>>::iterator& before);
		IPostEffect* AddPostEffectBack(std::unique_ptr<IPostEffect>& postEffect);
		GaussBlur* AddGaussBlur(std::unique_ptr<GaussBlur>& blur);
		void DeletePostEffect(const std::list<std::unique_ptr<IPostEffect>>::iterator& itr);

#pragma region Getter

		std::list<std::unique_ptr<IPostEffect>>::iterator GetPostEffectItr(const std::string& name);
		IPostEffect* GetPostEffect(const std::string& name);
		IPostEffect* GetBackBufferPtr();

#pragma endregion

#pragma region Setter

		void SetBackBuffer(IPostEffect* postEffect, int32_t mode = 0);

#pragma endregion
	};

}

