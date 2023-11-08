#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Quaternion.h"
#include <cassert>

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "QueryCallBack.h"
#include "XAudioManager.h"
#include "ImGuiManager.h"
#include "RayCast.h"

#include "PlayerIdleState.h"

using namespace CollAttribute;

void Player::StatusInitialize()
{
	//	初期ステート
	moveState_ = std::make_unique<PlayerIdleState>();
	PlayerMoveState::SetPlayer(this);

	avoid_.SetMaxTime(avoidCoolTime_);
	avoid_.Initialize();
	hp_.Initialize();
}

void Player::Initialize(IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	float radius = 0.6f;
	SetCollider(new SphereCollider(Vector3D(0.0f, radius, 0.0f), radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	StatusInitialize();
}

void Player::IsMovingUpdate()
{
	InputManager* input = InputManager::GetInstance();
	InputKeyboard* keyboard = input->GetKeyboard();
	int32_t frontKey = keyboard->GetKey(DIK_W) - keyboard->GetKey(DIK_S);
	int32_t sideKey = keyboard->GetKey(DIK_D) - keyboard->GetKey(DIK_A);

	InputJoypad* pad = input->GetPad();
	Vector2D inputVec = pad->GetThumbL().GetNormalize() + Vector2D(sideKey, frontKey);

	isMoving_ = inputVec.GetLength() != 0;

	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	moveVec_ = inputVec.y * camera->GetFrontVec() + inputVec.x * camera->GetRightVec();
	moveVec_.y = 0;

	//	Running
	if (input->GetTriggerKeyAndButton(DIK_LSHIFT, InputJoypad::B_Button)) {
		isRunning_ = !isRunning_;
	}
}

void Player::CalcModelFront()
{
	if (moveVec_.GetLength() != 0.0f) {
		Vector3D axis(0, 0, -1);
		Vector3D axis_(-1, 0, 0);
		float dot = axis_.dot(moveVec_);
		mat_.angle_.y = GetAngle(axis, moveVec_);
		if (dot < 0) mat_.angle_.y = -mat_.angle_.y;
	}
}

void Player::CoolTimeUpdate()
{
	avoid_.Update();
}

void Player::JumpUpdate()
{
	InputManager* input = InputManager::GetInstance();

	if (onGround_ == true) {
		if (input->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
			onGround_ = false;
			//	playerの状態に応じて変更予定
			moveY_ = jumpFirstSpd_;

			//	playJumpwav
		}
	}
	else {
		moveY_ = MyMath::mMax(moveY_ + fallAcc, fallVYMin);
	}
}

void Player::Update()
{
	//	死亡判定
	hp_.Update();

	//	死亡していたら
	if (hp_.GetIsAlive() == false) return;

	IsMovingUpdate();

	//	modelの正面(走らなくする判定もここで)
	CalcModelFront();

	CoolTimeUpdate();

	//	ジャンプの判定
	JumpUpdate();

	moveState_->Update();

	//	本移動
	mat_.trans_ += moveVec_ * spd_ + Vector3D(0.0f, moveY_, 0.0f);

	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	camera->SetTarget({ mat_.trans_.x,mat_.trans_.y + 1.0f,mat_.trans_.z });
	camera->MatUpdate();

	ColliderUpdate();
}

void Player::ImGuiMenuUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (imgui->BeginMenuBar()) {
		if (imgui->BeginMenu("File")) {
			if (imgui->MenuItem("Save")) SavePlayerStatus();
			imgui->EndMenu();
		}
		imgui->EndMenuBar();
	}
}

void Player::SavePlayerStatus()
{
}

void Player::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("PlayerStatus", true);

	ImGuiMenuUpdate();

	imgui->Text("angle : %.2f", mat_.angle_.y);

	if (imgui->CollapsingHeader("HP")) {
		//imgui->Text("Avoid : %s", avoid_.GetIsAvoiding() ? "TRUE" : "FALSE");
	}
	
	if (imgui->CollapsingHeader("Avoid")) {
		imgui->Text("Avoid : %s", avoid_.GetIsAvoiding() ? "TRUE" : "FALSE");
		imgui->InputInt("AvoidTime", avoidTime_);
		imgui->InputInt("AvoidCoolTime", avoidCoolTime_);
	}

	if(imgui->CollapsingHeader("Jump")) {
		imgui->Text("OnGround : %s", onGround_ ? "TRUE" : "FALSE");
		imgui->InputFloat("FallAcc", fallAcc);
		imgui->InputFloat("FallVYMin", fallVYMin);
		imgui->InputFloat("JumpFirstSpd", jumpFirstSpd_);
	}

	imgui->EndWindow();
}

void Player::CollisionUpdate()
{
	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallBack
	{
	public:
		Sphere* sphere = nullptr;
		Vector3D move;

		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const Vector3D up = { 0,1,0 };

			Vector3D rejectDir = info.reject;
			rejectDir.Normalize();
			float cos = rejectDir.dot(up);

			const float threshold = cosf(MyMath::ConvertToRad(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center_ += info.reject;
				move += info.reject;
			}

			return true;
		}
	};

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider_);
	assert(sphereCollider);
	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	mat_.trans_ += callback.move;

	MatUpdate();
	collider_->Update();

	Ray ray;
	ray.start = sphereCollider->center_;
	ray.start.y += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0 };
	RayCast raycastHit;

	if (onGround_) {
		const float adsDis = 0.2f;

		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.0f + adsDis)) {
			onGround_ = true;
			mat_.trans_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			ColliderUpdate();
			MatUpdate();
		}
		else {
			onGround_ = false;
			moveY_ = 0.0f;
		}
	}
	else if (moveY_ <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.0f)) {
			onGround_ = true;
			mat_.trans_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			ColliderUpdate();
			MatUpdate();
		}
	}
}

void Player::OnCollision(const CollisionInfo& info)
{
	(void)info;
	MatUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool Player::GetOnGround()
{
	return onGround_;
}

float Player::GetWalkSpd()
{
	return walkSpd_;
}

float Player::GetRunSpd()
{
	return runSpd_;
}

float Player::GetJumpingSpdDec()
{
	return jumpingSpdDec_;
}

bool Player::GetIsRunning()
{
	return isRunning_;
}

bool Player::GetIsAvoid()
{
	return avoid_.GetIsAvoiding();
}

bool Player::GetIsMoving()
{
	return isMoving_;
}

int32_t Player::GetAvoidTime()
{
	return avoidTime_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Player::SetMoveState(std::unique_ptr<PlayerMoveState>& moveState)
{
	moveState_ = std::move(moveState);
	moveState_->Initialize();
}

void Player::SetSpd(float spd)
{
	spd_ = spd;
}

void Player::SetIsAvoid(bool isAvoid)
{
	avoid_.SetIsAvoiding(isAvoid);
}
