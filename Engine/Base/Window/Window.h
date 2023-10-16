#pragma once
#define NOMINMAX
#include <Windows.h>
#include <stdint.h>

/**
* @file Window.h
* @brief 
*/

class Window
{
public:
	// ウィンドウサイズ
	static const int32_t sWIN_WIDTH = 1280; // 横幅
	static const int32_t sWIN_HEIGHT = 720; // 縦幅
	//static const int32_t sWIN_WIDTH = 1920; // 横幅
	//static const int32_t sWIN_HEIGHT = 1080; // 縦幅
	//static const int32_t sRESOLUTION_WID = 1920; // 横幅
	//static const int32_t sRESOLUTION_HEIGHT = 1080; // 縦幅
	static const int32_t sRESOLUTION_WID = 1280; // 横幅
	static const int32_t sRESOLUTION_HEIGHT = 720; // 縦幅
private:
	// ウィンドウクラスの設定
	WNDCLASSEX w_{};
	// ウィンドウオブジェクトの生成
	HWND hwnd_{};
	// メッセージ
	MSG msg_{};

private:	//	関数
	Window() {};
	~Window() {};

public:
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static Window* GetInstance();
	Window(const Window& obj) = delete;
	Window& operator=(const Window& obj) = delete;

	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();

	/**
	* @fn Finalize()
	* exe終了時に呼び出す関数
	*/
	void Finalize();

	bool MsgUpdate();
	bool EndLoop();

#pragma region Getter

	const HWND& GetHwnd() const { return hwnd_; }
	const WNDCLASSEX& GetWND() const { return w_;}

#pragma endregion
};

