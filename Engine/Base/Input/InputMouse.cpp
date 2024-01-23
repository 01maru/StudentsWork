#include "InputMouse.h"
#include "Window.h"
#include "ImGuiManager.h"
#include <cassert>

MNE::InputMouse::~InputMouse()
{
	//	範囲指定しない
	ClipCursor(NULL);
}

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::InputMouse::Initialize(IDirectInput8* directInput)
{
	Window* win = Window::GetInstance();
	HRESULT result;

	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));

	//	入力形成のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	//	排他制御のレベルセット
	result = mouse_->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::InputMouse::InputInfoUpdate()
{
	//	前フレームの情報
	prevClick_ = click_;

	//	マウス全体の情報
	mouse_->Acquire();
	//	ポーリング
	mouse_->Poll();
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &click_);
}

void MNE::InputMouse::LockCursorUpdate()
{
	//	カーソル固定中じゃなかったら処理しない
	if (isLockCursor_ == FALSE) return;

	//	中央に固定
	SetCursorWinCenter();
}

void MNE::InputMouse::UnLockCursorUpdate()
{
	//	カーソル固定中だったら処理しない
	if (isLockCursor_ == TRUE) return;

	//	カーソルの情報
	POINT cursor;
	//	ScreenLeftTop(0, 0)
	GetCursorPos(&cursor);
	//	WindowLeftTop(0, 0)
	ScreenToClient(Window::GetInstance()->GetHwnd(), &cursor);

	//	UpdateCursor
	cursor_.x = (float)cursor.x;
	cursor_.y = (float)cursor.y;
}

void MNE::InputMouse::MouseIsActiveUpdate()
{
	Vector3D inputV(static_cast<float>(click_.lX), static_cast<float>(click_.lY), static_cast<float>(click_.lZ));

	//	マウスの移動とホイールの入力があるか確認
	isActive_ = (inputV != Vector3D());

	//	既に入力があったら以下処理しない
	if (isActive_ == TRUE) return;

	//	ボタンの入力確認
	for (int32_t i = 0; i < ButtonNum; i++)
	{
		bool dikButton = click_.rgbButtons[i] & (0x80);

		//	入力があったら以降のボタンは確認しない
		if (dikButton == TRUE)
		{
			isActive_ = true;
			return;
		}
	}
}

void MNE::InputMouse::Update()
{
	//	入力状況更新
	InputInfoUpdate();

	//	カーソル固定時更新処理
	LockCursorUpdate();
	//	カーソル非固定時更新処理
	UnLockCursorUpdate();

	//	マウスの入力が行われたか確認
	MouseIsActiveUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void MNE::InputMouse::ImGuiUpdateCursor()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (!imgui->TreeNode("Cursor")) return;

	imgui->Text("IsActive : %s", isActive_ ? "True" : "False");

	imgui->Text("CursorPos  : (%.2f, %.2f)", cursor_.x, cursor_.y);
	imgui->Text("CursorMove : (%.2f, %.2f)", static_cast<float>(click_.lX), static_cast<float>(click_.lY));

	imgui->Text("ShowCursor : %s", showCursor_ ? "True" : "False");

	imgui->Text("LockCursor : %s", isLockCursor_ ? "True" : "False");
	imgui->CheckBox("LockCursor", isLockCursor_);

	imgui->TreePop();
}

void MNE::InputMouse::ImGuiUpdateClick()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (!imgui->TreeNode("Click")) return;

	imgui->Text("LeftClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(LeftClick) ? "True" : "False"
		, GetClickTrigger(LeftClick) ? "True" : "False"
		, GetClickRelease(LeftClick) ? "True" : "False");

	imgui->Text("RightClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(RightClick) ? "True" : "False"
		, GetClickTrigger(RightClick) ? "True" : "False"
		, GetClickRelease(RightClick) ? "True" : "False");

	imgui->Text("WheelClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(WheelClick) ? "True" : "False"
		, GetClickTrigger(WheelClick) ? "True" : "False"
		, GetClickRelease(WheelClick) ? "True" : "False");

	imgui->Text("SideClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(SideClick) ? "True" : "False"
		, GetClickTrigger(SideClick) ? "True" : "False"
		, GetClickRelease(SideClick) ? "True" : "False");

	imgui->Text("WheelMove  : %d", click_.lZ);

	imgui->TreePop();
}

void MNE::InputMouse::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (!imgui->TreeNode("Mouse")) return;

	ImGuiUpdateCursor();

	ImGuiUpdateClick();

	imgui->TreePop();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool MNE::InputMouse::GetIsActive()
{
	return isActive_;
}

bool MNE::InputMouse::GetClick(MouseButton type)
{
	return (click_.rgbButtons[type] & (0x80));
}

bool MNE::InputMouse::GetClickTrigger(MouseButton type)
{
	return (click_.rgbButtons[type] & (0x80)) && !(prevClick_.rgbButtons[type] & (0x80));
}

bool MNE::InputMouse::GetClickRelease(MouseButton type)
{
	return !(click_.rgbButtons[type] & (0x80)) && (prevClick_.rgbButtons[type] & (0x80));
}

const Vector2D& MNE::InputMouse::GetCursor()
{
	return cursor_;
}

Vector2D MNE::InputMouse::GetCursorMoveVec()
{
	return Vector2D(static_cast<float>(click_.lX), static_cast<float>(click_.lY));
}

int32_t MNE::InputMouse::GetWheel()
{
	return click_.lZ;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::InputMouse::SetCursorWinCenter()
{
	//	カーソルの情報
	POINT cursor;

	//	ウィンドウの中心座標
	Vector2D winCenter;
	winCenter.x = Window::sWIN_WIDTH / 2.0f;
	winCenter.y = Window::sWIN_HEIGHT / 2.0f;
	cursor.x = static_cast<LONG>(winCenter.x);
	cursor.y = static_cast<LONG>(winCenter.y);

	//	クライアント座標->スクリーン座標に変換
	ClientToScreen(Window::GetInstance()->GetHwnd(), &cursor);

	//	カーソル位置設定
	SetCursorPos((int32_t)cursor.x, (int32_t)cursor.y);
	cursor_ = winCenter;
}

void MNE::InputMouse::SetShowCursor(bool showCursor)
{
	//	変更なしだったら処理しない
	if (showCursor_ == showCursor) return;

	//	カーソル表示or非表示
	ShowCursor(showCursor_);
}

void MNE::InputMouse::SetLockCursor(bool lockCursor)
{
	if (isLockCursor_ == lockCursor) return;

	isLockCursor_ = lockCursor;

	if (isLockCursor_ == TRUE)
	{
		RECT rec;
		//	ウィンドウの大きさ取得
		GetWindowRect(Window::GetInstance()->GetHwnd(), &rec);

		Vector2D center((rec.right + rec.left) / 2.0f, (rec.bottom + rec.top) / 2.0f);
		float width = Window::sWIN_WIDTH / 2.0f;
		float height = Window::sWIN_HEIGHT / 2.0f;
		rec.left = (LONG)(center.x - width);
		rec.right = (LONG)(center.x + width);
		rec.top = (LONG)(center.y - height);
		rec.bottom = (LONG)(center.y + height);

		//	範囲指定
		ClipCursor(&rec);
	}
	else
	{
		//	範囲指定しない
		ClipCursor(NULL);
	}
}
