#include "ClearUI.h"
#include "SceneManager.h"

void ClearUI::Initialize()
{
	data_.LoadData("Clear");
}

void ClearUI::Update()
{
	if (isActive_ == false) return;

	if (data_.GetIsEndAnimation() == true)
	{
		isActive_ = false;

		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	data_.Update();
}

void ClearUI::Draw()
{
	if (isActive_ == false) return;
	data_.Draw();
}
