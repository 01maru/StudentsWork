#include "InputKeyboard.h"
#include "Window.h"
#include <cassert>

void MNE::InputKeyboard::Initialize(IDirectInput8* directInput)
{
	Window* win = Window::GetInstance();
	HRESULT result;

	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = keyboard_->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void MNE::InputKeyboard::Update()
{
	//	前フレームの情報取得
	for (size_t i = 0; i < sizeof(key_); i++)
	{
		prev_[i] = key_[i];
	}

	//	キー情報取得
	keyboard_->Acquire();
	//	全キーの入力情報取得
	keyboard_->GetDeviceState(sizeof(key_), key_);

	isActive_ = false;
	for (size_t i = 0; i < sizeof(key_); i++)
	{
		if (key_[i] != 0)
		{
			isActive_ = true;
			return;
		}
	}
}

bool MNE::InputKeyboard::GetIsActive()
{
	return isActive_;
}

bool MNE::InputKeyboard::GetKey(int key)
{
	return key_[key];
}

bool MNE::InputKeyboard::GetTrigger(int key)
{
	return key_[key] && !prev_[key];
}

bool MNE::InputKeyboard::GetRelease(int key)
{
	return prev_[key] && !key_[key];
}
