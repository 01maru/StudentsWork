#include "InputManager.h"
#include "Window.h"
#include "ImGuiManager.h"
#include "ImGuiController.h"
#include <cassert>

MNE::InputManager* MNE::InputManager::GetInstance()
{
	static InputManager instance;
	return &instance;
}

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::InputManager::Initialize()
{
	//	DirectInput初期化
	HRESULT result = DirectInput8Create(
		Window::GetInstance()->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	mouse_ = std::make_unique<InputMouse>();
	mouse_->Initialize(directInput_.Get());
	//	初期はマウス設定
	usePad_ = false;
	mouse_->SetCursorWinCenter();

	joypad_ = std::make_unique<InputJoypad>();

	keyboard_ = std::make_unique<InputKeyboard>();
	keyboard_->Initialize(directInput_.Get());

	explane_.LoadResources("ExplaneUI", "cursor.png");
	explane_.Initialize();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::InputManager::UsePadUpdate()
{
	//	パッド使用中
	if (usePad_ == TRUE)
	{
		//	キーボードかマウスの入力があったら
		if (keyboard_->GetIsActive() == TRUE ||
			mouse_->GetIsActive() == TRUE) {
			usePad_ = false;
		}
	}
	//	キーマウ使用中
	else
	{
		//	パッドの入力があったら
		if (joypad_->GetInputted() == TRUE) {
			usePad_ = true;
		}
	}
}

void MNE::InputManager::Update()
{
	bool prevUsePad = usePad_;

	mouse_->Update();

	joypad_->Update();

	keyboard_->Update();

	UsePadUpdate();

	explane_.Update(prevUsePad);
}

void MNE::InputManager::MatUpdate()
{
	explane_.MatUpdate();
}

void MNE::InputManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveInputManager()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("InputManager");

	imgui->Text("UsePad  : %s", usePad_ ? "True" : "False");

	imgui->SetSliderFloat("Sensitivity", sensitivity_, 0.01f, 0.01f, 1.0f);

	mouse_->ImGuiUpdate();

	joypad_->ImGuiUpdate();

	imgui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::InputManager::Draw()
{
	explane_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MNE::InputMouse* MNE::InputManager::GetMouse()
{
	return mouse_.get();
}

MNE::InputJoypad* MNE::InputManager::GetPad()
{
	return joypad_.get();
}

MNE::InputKeyboard* MNE::InputManager::GetKeyboard()
{
	return keyboard_.get();
}

bool MNE::InputManager::GetKeyAndButton(int key, InputJoypad::JoyPadButton button)
{
	return keyboard_->GetKey(key) || joypad_->GetButton(button);
}

bool MNE::InputManager::GetTriggerKeyAndButton(int key, InputJoypad::JoyPadButton button)
{
	return keyboard_->GetTrigger(key) || joypad_->GetButtonTrigger(button);
}

float MNE::InputManager::GetSensitivity()
{
	return sensitivity_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::InputManager::SetDrawExplane(bool draw)
{
	explane_.SetIsActive(draw);
}

void MNE::InputManager::SetNextTag(const std::string& nextTag, bool playAnimation, bool startingAnimation)
{
	explane_.SetNextAnimationTag(nextTag, playAnimation, startingAnimation);
}

void MNE::InputManager::SetSensitivity(float sens)
{
	sensitivity_ = sens;
}
