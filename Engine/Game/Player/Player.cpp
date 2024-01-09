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
#include "PlayerNoAttackState.h"

#include "UIData.h"
#include "UISprite.h"

#include "GameOverCamera.h"

#include "IGameState.h"

using namespace CollAttribute;

void Player::StatusInitialize()
{
	//	初期ステート
	moveState_ = std::make_unique<PlayerIdleState>();
	moveState_->Initialize();
	attackState_ = std::make_unique<PlayerNoAttackState>();

	avoidCT_.SetMaxTime(avoidCoolTime_);
	avoidCT_.Initialize();
	slowAtCT_.SetMaxTime(slowATCoolTime_);
	slowAtCT_.Initialize();
	hp_.Initialize();
}

void Player::Initialize(IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	mat_.scale_ = Vector3D(0.4f, 0.4f, 0.4f);
	float radius = 0.2f;
	Vector3D offset(0.0f, 0.2f, 0.0f);
	SetCollider(new SphereCollider(offset, radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	PlayerMoveState::SetPlayer(this);
	PlayerAttackState::SetPlayer(this);

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
	if (isMoving_ == true) {
		moveVec_ = inputVec.y * camera->GetFrontVec() + inputVec.x * camera->GetRightVec();
		moveVec_.y = 0;
	}

	//	Running
	if (input->GetTriggerKeyAndButton(DIK_LCONTROL, InputJoypad::B_Button)) {
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
	avoidCT_.Update();
	slowAtCT_.Update();
}

void Player::JumpUpdate()
{
	InputManager* input = InputManager::GetInstance();

	if (onGround_ == true) {
		if (input->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
			onGround_ = false;
			//	playerの状態に応じて変更予定
			moveY_ = jumpFirstSpd_;
			//SetAnimationIdx(1);
			//SetAnimationTimer(0);

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
	if (hp_.GetIsAlive() == false) {
		if (gameOver_ == false) {
			gameOver_ = true;
			pGameOverState_->Start();
		}
		return;
	}

	GetAnimation()->SetAnimatonTimer(static_cast<float>(animationTimer_++));

	IsMovingUpdate();

	//	modelの正面(走らなくする判定もここで)
	CalcModelFront();

	CoolTimeUpdate();

	//	ジャンプの判定
	JumpUpdate();

	moveState_->Update();

	//	本移動
	mat_.trans_ += moveVec_ * spd_;
	mat_.trans_.y += moveY_;

	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	Vector3D target = mat_.trans_;
	target += camera->GetFrontVec() * 3.0f;
	target.y += 2.0f;
	camera->SetTarget(target);
	//	eyeも動かす
	camera->SetEye(target - camera->GetDisEyeTarget() * camera->GetFrontVec());
	camera->MatUpdate();

	crossHair_.Update(mat_.trans_);

	attackState_->Update();

	rate_.Update();
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) {
		return bullet->GetIsActive() == false;
		});

	for (auto itr = bullets_.begin(); itr != bullets_.end(); itr++)
	{
		itr->get()->Update();
	}

	ColliderUpdate();

	MatUpdate();
	Object3D::ColliderUpdate();
	mat_.trans_ = CollisionManager::GetInstance()->CollisionStage(*dynamic_cast<SphereCollider*>(collider_));
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

	crossHair_.ImGuiUpdate();

	imgui->Text("animationIdx : %d", animationIdx_);
	imgui->Text("animationTimer : %d", animationTimer_);
	imgui->Text("angle : %.2f", mat_.angle_.y);
	imgui->Text("bullet : %d", bullets_.size());
	imgui->Text("bulletRate : %d", rate_.GetFrameCount());

	if (imgui->CollapsingHeader("HP")) {
		imgui->Text("isAlive : %s", hp_.GetIsAlive() ? "TRUE" : "FALSE");
		imgui->Text("HP : %d", hp_.GetHP());

		int32_t maxHP = hp_.GetMaxHP();
		imgui->InputInt("MaxHP", maxHP);
		hp_.SetMaxHP(maxHP);

		int32_t debugDamage = 10;
		if (imgui->SetButton("GetDamage")) {
			hp_.DecHp(debugDamage);
		}
	}

	if (imgui->CollapsingHeader("Move")) {
		imgui->Text("IsMoving : %s", isMoving_ ? "TRUE" : "FALSE");
		imgui->Text("IsRunning : %s", isRunning_ ? "TRUE" : "FALSE");
		imgui->Text("Spd : %.2f", spd_);

		imgui->InputFloat("walkSpd", walkSpd_);
		imgui->InputFloat("runSpd", runSpd_);
		imgui->InputFloat("jumpingSpdDec", jumpingSpdDec_);
	}
	
	if (imgui->CollapsingHeader("Avoid")) {
		imgui->Text("AvoidIsActive : %s", avoidCT_.GetIsActive() ? "TRUE" : "FALSE");
		imgui->Text("Avoid : %s", avoidCT_.GetIsAvoiding() ? "TRUE" : "FALSE");
		imgui->InputInt("AvoidAccTime", avoidAccTime_);
		imgui->InputInt("AvoidDecTime", avoidDecTime_);
		imgui->InputInt("AvoidCoolTime", avoidCoolTime_);
		avoidCT_.ImGuiUpdate();
	}

	if(imgui->CollapsingHeader("Jump")) {
		imgui->Text("OnGround : %s", onGround_ ? "TRUE" : "FALSE");
		imgui->InputFloat("FallAcc", fallAcc);
		imgui->InputFloat("FallVYMin", fallVYMin);
		imgui->InputFloat("JumpFirstSpd", jumpFirstSpd_);
		imgui->Text("MoveY : %.2f", moveY_);
	}

	if (imgui->CollapsingHeader("State")) {
		moveState_->ImGuiUpdate();
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
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_ENEMYS);
	mat_.trans_ += callback.move;

	MatUpdate();
	collider_->Update();

	Ray ray;
	ray.start = sphereCollider->center_;
	ray.start.y += sphereCollider->GetRadius();
	Vector3D downVec(0, -1, 0);
	ray.dir = downVec;
	RayCast raycastHit;

	float diameter = sphereCollider->GetRadius() * 2.0f;
	if (onGround_) {
		const float adsDis = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			diameter + adsDis)) {
			onGround_ = true;
			mat_.trans_.y -= (raycastHit.distance - diameter);
			Object3D::ColliderUpdate();
			MatUpdate();
		}
		else {
			onGround_ = false;
			moveY_ = 0.0f;
		}
	}
	else if (moveY_ <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			diameter)) {
			onGround_ = true;
			mat_.trans_.y -= (raycastHit.distance - diameter);
			Object3D::ColliderUpdate();
			MatUpdate();
		}
	}
}

void Player::OnCollision(CollisionInfo& info)
{
	(void)info;
}

void Player::DrawBullets()
{
	for (auto itr = bullets_.begin(); itr != bullets_.end(); itr++)
	{
		itr->get()->Draw();
	}
}

void Player::DrawUI()
{
	crossHair_.Draw();
	hp_.Draw();
	avoidCT_.Draw();
	slowAtCT_.Draw();
}

void Player::AddBullet(std::unique_ptr<Bullet>& bullet)
{
	bullets_.push_back(std::move(bullet));
}

void Player::StartSlowAtCT()
{
	slowAtCT_.StartCount();
}

void Player::DecHP(int32_t damage)
{
	hp_.DecHp(damage);
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
	return avoidCT_.GetIsAvoiding();
}

bool Player::GetIsMoving()
{
	return isMoving_;
}

int32_t Player::GetAvoidAccTime()
{
	return avoidAccTime_;
}

int32_t Player::GetAvoidDecTime()
{
	return avoidDecTime_;
}

float Player::GetSpd()
{
	return spd_;
}

float Player::GetAvoidMaxSpd()
{
	return avoidMaxSpd_;
}

Vector3D Player::GetBulletFront()
{
	return crossHair_.GetDir();
}

bool Player::GetRateCountIsActive()
{
	return rate_.GetIsActive();
}

bool Player::GetSlowAtIsActive()
{
	return slowAtCT_.GetIsActive();
}

int32_t Player::GetBulletRate()
{
	return bulletRate_;
}

bool Player::GetIsAlive()
{
	return hp_.GetIsAlive();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Player::SetMoveState(std::unique_ptr<PlayerMoveState>& moveState)
{
	moveState_ = std::move(moveState);
	moveState_->Initialize();
}

void Player::SetAttackState(std::unique_ptr<PlayerAttackState>& attackState)
{
	attackState_ = std::move(attackState);
	attackState_->Initialize();
}

void Player::SetSpd(float spd)
{
	spd_ = spd;
}

void Player::SetIsAvoid(bool isAvoid)
{
	avoidCT_.SetIsAvoiding(isAvoid);
}

void Player::SetIsRunning(bool isRunning)
{
	isRunning_ = isRunning;
}

void Player::SetBulletRate(int32_t rate)
{
	rate_.SetMaxFrameCount(rate);
}

void Player::StartRateCount()
{
	rate_.StartCount();
}

void Player::SetCrossHairSprite(const Sprite& sprite)
{
	crossHair_.SetSprite(sprite);
}

void Player::SetHPBarSprite(const Sprite& sprite)
{
	hp_.SetSprite(sprite);

	Vector3D green(0.0f, 1.0f, 0.0f);
	hp_.SetBarColor(green);
}

void Player::SetAvoidCoolSprite(const Sprite& sprite, const Sprite& text)
{
	avoidCT_.SetSprite(sprite);
	avoidCT_.SetTextSprite(text);
}

void Player::SetSlowAtCoolSprite(const Sprite& sprite, const Sprite& text)
{
	slowAtCT_.SetSprite(sprite);
	slowAtCT_.SetTextSprite(text);
}

void Player::SetGameOverState(IGameState* gameOverState)
{
	pGameOverState_ = gameOverState;
}
