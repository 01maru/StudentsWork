﻿#pragma once
#include "InputMouse.h"
#include "InputJoypad.h"
#include "InputKeyboard.h"
#include <memory>

class InputManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDirectInput8> directInput_ = nullptr;

	std::unique_ptr<InputMouse> mouse_;
	std::unique_ptr<InputJoypad> joypad_;
	std::unique_ptr<InputKeyboard> keyboard_;

	float sensitivity_ = 1.0f;

private:
	InputManager() {};
	~InputManager() {};
public:
	static InputManager* GetInstance();
	InputManager(const InputManager& obj) = delete;
	InputManager& operator=(const InputManager& obj) = delete;

	void Initialize();
	void Update();
	void ImGuiUpdate();

	//	Getter
	InputMouse* GetMouse() { return mouse_.get(); }
	InputJoypad* GetPad() { return joypad_.get(); }
	InputKeyboard* GetKeyboard() { return keyboard_.get(); }

	bool GetKeyAndButton(int key, InputJoypad::JoyPadButton button);
	bool GetTriggerKeyAndButton(int key, InputJoypad::JoyPadButton button);
	float GetSensitivity() { return sensitivity_; }

	//	Setter
	void SetSensitivity(float sens) { sensitivity_ = sens; }
};

