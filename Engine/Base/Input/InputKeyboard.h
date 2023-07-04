#pragma once
#include <dinput.h>
#include <wrl.h>
#include <stdint.h>

class InputKeyboard
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	int8_t key_[256] = {};
	int8_t prev_[256] = {};

public:		//	関数
	void Initialize(IDirectInput8* directInput);
	void Update();

	//	Getter
	bool GetKey(int key);
	bool GetTrigger(int key);
	bool GetRelease(int key);
};

