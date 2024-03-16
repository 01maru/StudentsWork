#include "PostEffectManager.h"

MNE::PostEffectManager* MNE::PostEffectManager::GetInstance()
{
	static PostEffectManager instance;
	return &instance;
}

void MNE::PostEffectManager::Update()
{
	for (auto& i : postEffects_)
	{
		i->Update();
	}
}

MNE::IPostEffect* MNE::PostEffectManager::AddPostEffect(std::unique_ptr<IPostEffect>& postEffect, const std::list<std::unique_ptr<IPostEffect>>::iterator& before)
{

	//	挿入
	postEffects_.insert(before, std::move(postEffect));

	return (*std::next(before, 1)).get();
}

MNE::IPostEffect* MNE::PostEffectManager::AddPostEffectBack(std::unique_ptr<IPostEffect>& postEffect)
{
	postEffects_.push_back(std::move(postEffect));
	
	return postEffects_.back().get();
}

void MNE::PostEffectManager::DeletePostEffect(const std::list<std::unique_ptr<IPostEffect>>::iterator& itr)
{
	//	削除
	postEffects_.erase(itr);
}

std::list<std::unique_ptr<MNE::IPostEffect>>::iterator MNE::PostEffectManager::GetPostEffectItr(const std::string& name)
{
	for (auto itr = postEffects_.begin(); itr != postEffects_.end(); itr++)
	{
		if (itr->get()->GetName() == name)
		{
			return itr;
		}
	}

	return postEffects_.end();
}

MNE::IPostEffect* MNE::PostEffectManager::GetPostEffect(const std::string& name)
{
	for (auto& i : postEffects_)
	{
		if (i->GetName() == name)
		{
			return i.get();
		}
	}

	return nullptr;
}

void MNE::PostEffectManager::DrawBack()
{
	auto end = postEffects_.back().get();
	end->Draw();
}
