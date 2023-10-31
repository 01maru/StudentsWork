#pragma once
#include "InputMouse.h"
#include "InputJoypad.h"
#include "InputKeyboard.h"
#include <memory>

/**
* @file InputManager.h
* @brief コントローラー、マウス、キーボードすべての入力処理をまとめて管理しているファイル
*/

class InputManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDirectInput8> directInput_ = nullptr;

	std::unique_ptr<InputMouse> mouse_;
	std::unique_ptr<InputJoypad> joypad_;
	std::unique_ptr<InputKeyboard> keyboard_;

	float sensitivity_ = 0.2f;

private:
	InputManager() {};
	~InputManager() {};
public:
	static InputManager* GetInstance();
	//	コピーコンストラクタ無効
	InputManager(const InputManager& obj) = delete;
	//	代入演算子無効
	InputManager& operator=(const InputManager& obj) = delete;

	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();
    /**
    * @fn Update()
    * 更新処理関数
    */
    void Update();
    /**
    * @fn Update()
    * ImGui更新処理関数
    */
    void ImGuiUpdate();

#pragma region Getter

	InputMouse* GetMouse() { return mouse_.get(); }
	InputJoypad* GetPad() { return joypad_.get(); }
	InputKeyboard* GetKeyboard() { return keyboard_.get(); }

	bool GetKeyAndButton(int key, InputJoypad::JoyPadButton button);
	bool GetTriggerKeyAndButton(int key, InputJoypad::JoyPadButton button);
	float GetSensitivity() { return sensitivity_; }

#pragma endregion

#pragma region Setter

	void SetSensitivity(float sens) { sensitivity_ = sens; }

#pragma endregion
};

