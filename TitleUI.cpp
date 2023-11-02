#include "TitleUI.h"

#include "InputManager.h"
#include "XAudioManager.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include "Easing.h"

using namespace Easing;

void TitleUI::Initialize()
{
	LoadResources();
	option_.Initialize("Option");

	cursor_.Initialize(TextureManager::GetInstance()->GetTextureGraph("select.png"));
	cursor_.SetPosition(Vector2D(200, 420));
	cursor_.SetAnchorPoint(Vector2D(0.5f, 0.5f));

	selectCounter_.Initialize(40, true, true);
	selectCounter_.SetIsEndless(true);
	selectCounter_.SetIsActive(true);
}

void TitleUI::Finalize()
{
}

void TitleUI::LoadResources()
{
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
	xAudioMan->LoadSoundWave("decision.wav");

	TextureManager::GetInstance()->AsyncLoadTextureGraph("select.png");
	data_.LoadData("TitleScene");
}

void TitleUI::TitleInputUpdate(bool selectButton)
{
	//	オプションシーンだったら
	if (option_.GetIsActive() == true) return;

	//	ボタンを選択したら
	if (selectButton)
	{
		//	決定音再生
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		std::string buttonName = data_.GetSelectName();
		if (buttonName == "Start") {
			SceneManager::GetInstance()->SetNextScene("GAMESCENE");
		}

		else if (buttonName == "Option") {
			option_.SetIsActive(true);
			option_.ResetSelectButton();
		}

		else if (buttonName == "Quit") {
			SceneManager::GetInstance()->GameLoopEnd();
		}
	}

	data_.InputUpdate();
}

void TitleUI::TitleUpdate(bool selectButton)
{
	TitleInputUpdate(selectButton);

	data_.Update();
}

void TitleUI::OptionUpdate(bool selectButton)
{
	option_.InputUpdate(selectButton);
	option_.Update();
}

void TitleUI::CursorUpdate()
{
	float size = Easing::EaseIn(1.0f, 1.05f, selectCounter_.GetCountPerMaxCount(), 2);
	Vector2D cursorSize(298, 82);
	cursor_.SetSize(cursorSize * size);

	cursor_.SetPosition(data_.GetSelectPosition());
}

void TitleUI::Start()
{
	data_.Initialize();
}

void TitleUI::Update()
{
	bool isSelect = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	selectCounter_.Update();

	TitleUpdate(isSelect);

	OptionUpdate(isSelect);

	CursorUpdate();

	cursor_.Update();
}

void TitleUI::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();
	std::string name = data_.GetSelectName();
	imgui->Text("mord : %s", name.c_str());
	option_.ImGuiUpdate();
}

void TitleUI::Draw()
{
	data_.Draw();

	option_.Draw();

	cursor_.Draw();
}
