#pragma once
#include "IPostEffect.h"
#include <list>
#include <memory>

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

	private:
		std::list<std::unique_ptr<IPostEffect>> postEffects_;

	public:
		IPostEffect* AddPostEffect(std::unique_ptr<IPostEffect>& postEffect, const std::list<std::unique_ptr<IPostEffect>>::iterator& before);
		IPostEffect* AddPostEffectBack(std::unique_ptr<IPostEffect>& postEffect);
		void DeletePostEffect(const std::list<std::unique_ptr<IPostEffect>>::iterator& itr);
		std::list<std::unique_ptr<IPostEffect>>::iterator GetPostEffectItr(const std::string& name);
		IPostEffect* GetPostEffect(const std::string& name);
		void DrawBack();
	};

}

