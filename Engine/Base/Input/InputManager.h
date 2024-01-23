#pragma once
#include "InputMouse.h"
#include "InputJoypad.h"
#include "InputKeyboard.h"
#include "ExplanatoryText.h"
#include <memory>

/**
* @file InputManager.h
* @brief コントローラー、マウス、キーボードすべての入力処理をまとめて管理しているファイル
*/

namespace MNE
{

	class InputManager
	{
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

		void Draw();

	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<IDirectInput8> directInput_ = nullptr;

		std::unique_ptr<InputMouse> mouse_;
		std::unique_ptr<InputJoypad> joypad_;
		std::unique_ptr<InputKeyboard> keyboard_;
		
		bool usePad_ = false;

		ExplanatoryText explane_;

		float sensitivity_ = 0.2f;

	private:
		void UsePadUpdate();

	public:
	#pragma region Getter

		bool GetUsePad() { return usePad_; }
		InputMouse* GetMouse();
		InputJoypad* GetPad();
		InputKeyboard* GetKeyboard();

		bool GetKeyAndButton(int key, InputJoypad::JoyPadButton button);
		bool GetTriggerKeyAndButton(int key, InputJoypad::JoyPadButton button);
		float GetSensitivity();

	#pragma endregion

	#pragma region Setter

		void SetDrawExplane(bool draw);
		void SetNextTag(const std::string& nextTag, bool playAnimation, bool startingAnimation);
		void SetSensitivity(float sens);

	#pragma endregion
	};

}
