#include "PostEffectManager.h"

MNE::PostEffectManager* MNE::PostEffectManager::GetInstance()
{
	static PostEffectManager instance;
	return &instance;
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::PostEffectManager::Update()
{
	for (auto& i : postEffects_)
	{
		i->Update();
	}
}

void MNE::PostEffectManager::ImGuiUpdate()
{
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::PostEffectManager::DrawBackBuffer()
{
	auto backBuffer = backBuffer_;

	if (backBuffer == nullptr) {
		backBuffer = postEffects_.back().get();
	}

	backBuffer->Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Add&Delete
//-----------------------------------------------------------------------------

MNE::IPostEffect* MNE::PostEffectManager::AddPostEffect(std::unique_ptr<IPostEffect>& postEffect, const std::list<std::unique_ptr<IPostEffect>>::iterator& before)
{
	//	挿入
	postEffects_.insert(before, std::move(postEffect));

	return (*std::next(before)).get();
}

MNE::IPostEffect* MNE::PostEffectManager::AddPostEffectBack(std::unique_ptr<IPostEffect>& postEffect)
{
	postEffects_.push_back(std::move(postEffect));
	
	return postEffects_.back().get();
}

MNE::GaussBlur* MNE::PostEffectManager::AddGaussBlur(std::unique_ptr<GaussBlur>& blur)
{
	gaussBlurs_.push_back(std::move(blur));

	return gaussBlurs_.back().get();
}

void MNE::PostEffectManager::DeletePostEffect(const std::list<std::unique_ptr<IPostEffect>>::iterator& itr)
{
	//	削除
	postEffects_.erase(itr);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

std::list<std::unique_ptr<MNE::IPostEffect>>::iterator MNE::PostEffectManager::GetPostEffectItr(const std::string& name)
{
	for (auto itr = postEffects_.begin(); itr != postEffects_.end(); ++itr)
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

MNE::IPostEffect* MNE::PostEffectManager::GetBackBufferPtr()
{
	return backBuffer_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::PostEffectManager::SetBackBuffer(IPostEffect* postEffect, int32_t mode)
{
	backBuffer_ = postEffect;
	backBufferMode_ = mode;
}
