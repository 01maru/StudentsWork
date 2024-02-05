#include "GameCamera.h"
#include "ImGuiManager.h"
#include "InputManager.h"
#include "Easing.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void GameCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	InputManager::GetInstance()->GetMouse()->SetLockCursor(true);
	targeting_ = false;

	minAngleRange_ = MyMath::ConvertToRad(MIN_ANGLE_Y);
	maxAngleRange_ = MyMath::ConvertToRad(MAX_ANGLE_Y);

	ICamera::Initialize(eye, target, up);
}

void GameCamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	InputManager::GetInstance()->GetMouse()->SetLockCursor(true);
	targeting_ = false;
	isActive_ = true;

	minAngleRange_ = MyMath::ConvertToRad(MIN_ANGLE_Y);
	maxAngleRange_ = MyMath::ConvertToRad(MAX_ANGLE_Y);

	ICamera::Initialize(frontVec, center, dis);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void GameCamera::TargetCameraUpdate()
{
	if (targeting_ == FALSE)
	{
		//	キー入力されていたら
		if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_Q, InputJoypad::RStick_Button) == TRUE)
		{
			targeting_ = TRUE;

			//	開始位置と終了位置保存
			startFront_ = Quaternion(0.0f, frontVec_);
			endFront_ = Quaternion(0.0f, (*pEnemyPos_ - *pPlayerPos_).GetNormalize());
			//	移動量に応じて終了時間調節
			const float DOT_MIN = -1.0f;
			float dot = startFront_.Dot(endFront_) - DOT_MIN;	//	単位ベクトル同士の内積値は-1~1で0~2に変更したい
			const float maxV = 2.0f;							//	↑より内積値は最大値:1 と 最小値:-1 の差は2
			//	移動量による割合
			float rate = (maxV - dot) / maxV;
			float maxTime = minMoveTime_ + maxMoveTime_ * rate;
			counter_.Initialize(static_cast<int32_t>(maxTime), true);
			//	カウンタースタート
			counter_.StartCount();
		}
		else
		{
			//	押されていなかったら以降の処理しない
			return;
		}
	}

	//	移動終了していたら
	if (counter_.GetIsActive() == FALSE)
	{
		//	ターゲット終了
		targeting_ = FALSE;
		return;
	}

	//	カウンター更新
	counter_.Update();

	//	正面ベクトル更新
	Quaternion q = CameraSlerp(startFront_, endFront_, counter_.GetCountPerMaxCount());
	frontVec_ = q.GetVector3();
	frontVec_.Normalize();
	//	上方向は常に(0, 1, 0)
	const Vector3D axisY(0, 1, 0);
	up_ = axisY;
	//	右方向ベクトル更新
	rightVec_ = -frontVec_.cross(up_);
	rightVec_.Normalize();

	//	座標更新
	eye_ = target_ - disEyeTarget_ * frontVec_;
}

void GameCamera::UnTargetUpdate()
{
	//	ターゲット中だったら処理しない
	if (targeting_ == TRUE) return;

	InputManager* input = InputManager::GetInstance();
	InputJoypad* pad = input->GetPad();
	InputMouse* mouse = input->GetMouse();

	//	PadInfo
	Vector2D padVec = pad->GetThumbR();
	padVec.y = -padVec.y;
	float maxLen = static_cast<float>(pad->GetMaxThumbRange() - pad->GetThumbRDeadZone());
	padVec /= maxLen;
	padVec *= padSpd_;

	//	MouseInfo
	Vector2D mouseVec = mouse->GetCursorMoveVec();
	mouseVec /= mouseMaxRad_;

	//	Mouse&Pad
	Vector2D moveVec = padVec + mouseVec;
	float len = moveVec.GetLength();
	const float LEN_MAX = 1.0f;
	len = MyMath::mMin(len, LEN_MAX);
	moveVec.Normalize();
	moveVec *= input->GetSensitivity() * len;

	//	座標更新
	const Vector3D axisY(0, 1, 0);
	//	現在の縦軸と正面ベクトルの角度
	float angleY = GetAngle(axisY, frontVec_);
	//	入力による移動をした際に制限範囲外に行っていたら範囲内に戻す処理
	moveVec.y = MyMath::mClamp(minAngleRange_ - angleY, maxAngleRange_ - angleY, moveVec.y);

	//	縦軸の移動
	Quaternion upMove = MakeAxisAngle(rightVec_, moveVec.y);
	//	横軸の移動
	Quaternion rightMove = MakeAxisAngle(axisY, moveVec.x);
	//	合成した全体の移動
	Quaternion qMove = upMove * rightMove;
	//	方向ベクトルを移動させる
	rightVec_ = RotateVector(rightVec_, qMove);
	rightVec_.Normalize();
	frontVec_ = RotateVector(frontVec_, qMove);
	frontVec_.Normalize();
	//	上方向は常に(0, 1, 0)
	up_ = axisY;
	up_.Normalize();

	//	座標更新
	eye_ = target_ - disEyeTarget_ * frontVec_;
	//	座標更新後の方向ベクトル更新
	CalcDirectionVec();
}

void GameCamera::Update()
{
	//	実行中なら
	if (isActive_ == TRUE)
	{
		TargetCameraUpdate();

		UnTargetUpdate();
	}

	ICamera::Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void GameCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	//	ターゲット中かどうか
	imgui->Text("LockOn : %s", targeting_ ? "TRUE" : "FALSE");
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void GameCamera::SetEnemyPos(Vector3D* pos)
{
	pEnemyPos_ = pos;
}

void GameCamera::SetPlayerPos(Vector3D* pos)
{
	pPlayerPos_ = pos;
}

void GameCamera::SetIsActive(bool isActive)
{
	isActive_ = isActive;
}
