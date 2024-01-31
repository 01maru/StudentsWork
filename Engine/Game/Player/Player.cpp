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

#include "GameOverUI.h"

using namespace CollAttribute;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void Player::StatusInitialize()
{
	//	初期ステート
	moveState_ = std::make_unique<PlayerIdleState>();
	moveState_->Initialize();
	attackState_ = std::make_unique<PlayerNoAttackState>();

	//	Skills
	nBulletSprite_.Initialize();
	avoidCTSprite_.Initialize();
	slowAtCTSprite_.Initialize();

	//	HP初期化
	hp_.Initialize();
}

void Player::Initialize(MNE::IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	float radius = 0.5f;
	offset_ = Vector3D(0.0f, radius, 0.0f);
	SetCollider(new SphereCollider(offset_, radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	PlayerMoveState::SetPlayer(this);
	PlayerAttackState::SetPlayer(this);

	StatusInitialize();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void Player::CalcModelFront(const Vector2D& inputVec)
{
	//	移動してなかったら以下の処理はしない
	if (isMoving_ == FALSE) return;

	//	移動方向ベクトル変更
	ICamera* camera = CameraManager::GetInstance()->GetCamera();

	moveVec_ = inputVec.y * camera->GetFrontVec() + inputVec.x * camera->GetRightVec();
	moveVec_.y = 0;
	//Vector3D inputMoveVec = inputVec.y * camera->GetFrontVec() + inputVec.x * camera->GetRightVec();
	//inputMoveVec.y = 0;

	//float dot = moveVec_.dot(inputMoveVec);
	//dot = (-dot + 1.0f) / 2.0f;		//	範囲を0.0f~1.0fに変更し、移動量が大きいときにdot値が大きくなるように変更

	////	回転する角度を決める
	//float angle = dot * maxAngle_;
	//angle = MyMath::ConvertToRad(angle);

	////	回転する向きを決める
	//Vector3D cross = moveVec_.cross(inputMoveVec);

	//if (cross.y < 0.0f)
	//{
	//	angle = -angle;
	//}

	////	Y軸回りに回転
	//Quaternion axisY = MakeAxisAngle(Vector3D(0, 1, 0), angle);

	//moveVec_ = RotateVector(inputMoveVec, axisY);

	//	モデルの向き計算
	Vector3D axisZ(0, 0, -1);
	Vector3D axisX(-1, 0, 0);
	float dot = axisX.dot(moveVec_);
	mat_.angle_.y = GetAngle(axisZ, moveVec_);
	if (dot < 0) mat_.angle_.y = -mat_.angle_.y;
}

void Player::IsMovingUpdate()
{
	InputManager* input = InputManager::GetInstance();
	InputKeyboard* keyboard = input->GetKeyboard();
	int32_t frontInput = keyboard->GetKey(DIK_W) - keyboard->GetKey(DIK_S);
	int32_t sideInput = keyboard->GetKey(DIK_D) - keyboard->GetKey(DIK_A);

	//	パッドでの移動入力
	Vector2D inputVec = input->GetPad()->GetThumbL().GetNormalize();
	//	パッド+キーでの入力
	inputVec += Vector2D(sideInput, frontInput);

	//	移動しているか判定
	isMoving_ = inputVec.GetLength() != 0;

	//	モデルの正面&移動方向計算
	CalcModelFront(inputVec);
}

void Player::InputUpdate()
{
	InputManager* input = InputManager::GetInstance();

	//	走っているか判定(攻撃中は歩くように)
	if (input->GetTriggerKeyAndButton(DIK_LCONTROL, InputJoypad::B_Button)) {
		isRunning_ = !isRunning_;
	}

	//	スライディングするか
	if (input->GetTriggerKeyAndButton(DIK_LSHIFT, InputJoypad::Left_Button) &&
		avoidCTSprite_.GetIsActive())
	{
		avoidCTSprite_.StartCount();
		avoiding_ = true;
	}
}

void Player::SkillsUpdate()
{
	nBulletSprite_.Update();
	avoidCTSprite_.Update();
	slowAtCTSprite_.Update();
}

void Player::JumpUpdate()
{
	InputManager* input = InputManager::GetInstance();

	if (onGround_ == true) {
		if (input->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
			onGround_ = false;
			//	playerの状態に応じて変更予定
			moveY_ = jumpFirstSpd_;
			SetAnimationIdx("jumpUp");
			SetAnimationTimer(0);

			//	playJumpwav
		}
	}
	else {
		float moveY = moveY_ + fallAcc;
		if (moveY_ > 0 && moveY <= 0)
		{
			SetAnimationIdx("jumpDown");
			SetAnimationTimer(0);
		}
		moveY_ = MyMath::mMax(moveY, fallVYMin);
	}
}

void Player::Update()
{
	if (isActive_ == FALSE) return;

	//	HPバーのアニメーション更新
	hp_.Update();

	GetAnimation()->SetAnimeTimer(static_cast<float>(animationTimer_++));

	//	死亡していたら
	if (hp_.GetIsAlive() == false) {
		if (gameOver_ == false) {
			gameOver_ = true;
			pGameOverState_->Start();
			pGameOverState_->SetCameraPosData(mat_.trans_);
			SetAnimationIdx("Death");
			SetAnimationTimer(0);
			GetAnimation()->SetIsLoop(false);
		}
		return;
	}

	IsMovingUpdate();

	InputUpdate();

	SkillsUpdate();

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

	crossHair_.Update(mat_.trans_ + offset_);

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
	mat_.trans_ = CollisionManager::GetInstance()->CollisionStage(*dynamic_cast<SphereCollider*>(collider_)) - offset_;
}

//-----------------------------------------------------------------------------
// [SECTION] CollisionUpdate
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void Player::ImGuiMenuUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (imgui->BeginMenuBar()) {
		if (imgui->BeginMenu("File")) {
			if (imgui->MenuItem("Load")) HotLoadStatus();
			if (imgui->MenuItem("Save")) SaveData();
			imgui->EndMenu();
		}
		imgui->EndMenuBar();
	}
}

void Player::HotLoadStatus()
{
	LoadData();

	avoidCTSprite_.SetMaxTime(avoidCoolTime_);
	slowAtCTSprite_.SetMaxTime(slowATCoolTime_);

	hp_.SetMaxHP(maxHP_);
}

void Player::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("PlayerStatus", true);

	ImGuiMenuUpdate();

	crossHair_.ImGuiUpdate();

	if (imgui->CollapsingHeader("Model")) {
		imgui->Text("animationTimer : %d", animationTimer_);
		imgui->Text("angle : %.2f", mat_.angle_.y);
	}

	if (imgui->CollapsingHeader("Attack")) {
		imgui->Text("bullet : %d", bullets_.size());
		imgui->Text("bulletRate : %d", rate_.GetFrameCount());
		imgui->Text("slowAT : %s", slowAtCTSprite_.GetIsActive() ? "TRUE" : "FALSE");

		ImGuiAttackUpdate();
	}

	if (imgui->CollapsingHeader("HP")) {
		imgui->Text("isAlive : %s", hp_.GetIsAlive() ? "TRUE" : "FALSE");
		imgui->Text("HP : %d", hp_.GetHP());

		ImGuiHPUpdate();
		hp_.SetMaxHP(maxHP_);

		int32_t debugDamage = 10;
		if (imgui->SetButton("GetDamage")) {
			hp_.DecHp(debugDamage);
		}
	}

	if (imgui->CollapsingHeader("Move")) {
		imgui->Text("IsMoving : %s", isMoving_ ? "TRUE" : "FALSE");
		imgui->Text("IsRunning : %s", isRunning_ ? "TRUE" : "FALSE");
		imgui->Text("Spd : %.2f", spd_);

		ImGuiMoveUpdate();
	}
	
	if (imgui->CollapsingHeader("Avoid")) {
		imgui->Text("AvoidIsActive : %s", avoidCTSprite_.GetIsActive() ? "TRUE" : "FALSE");
		imgui->Text("Avoid : %s", avoiding_ ? "TRUE" : "FALSE");

		ImGuiAvoidUpdate();
		//avoidCT_.ImGuiUpdate();
	}

	if(imgui->CollapsingHeader("Jump")) {
		imgui->Text("OnGround : %s", onGround_ ? "TRUE" : "FALSE");
		imgui->Text("MoveY : %.2f", moveY_);

		ImGuiJumpUpdate();
	}

	if (imgui->CollapsingHeader("State")) {
		moveState_->ImGuiUpdate();
	}

	imgui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

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

	//	Skills
	nBulletSprite_.Draw();
	avoidCTSprite_.Draw();
	slowAtCTSprite_.Draw();
}

void Player::AddBullet(std::unique_ptr<Bullet>& bullet)
{
	bullets_.push_back(std::move(bullet));
}

void Player::StartSlowAtCT()
{
	slowAtCTSprite_.StartCount();
}

void Player::StartAvoidCT()
{
	avoidCTSprite_.StartCount();
}

void Player::DecHP(int32_t damage)
{
	hp_.DecHp(damage);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector3D Player::GetCenterPos()
{
	return mat_.trans_ + offset_;
}

bool Player::GetOnGround()
{
	return onGround_;
}

bool Player::GetIsRunning()
{
	return isRunning_;
}

bool Player::GetIsAvoid()
{
	return avoiding_;
}

bool Player::GetIsMoving()
{
	return isMoving_;
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
	return slowAtCTSprite_.GetIsActive();
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
	avoiding_ = isAvoid;
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

void Player::SetUIInfo(MNE::UIData& uiData)
{
	//	HP
	UISprite* gameUISprite = uiData.GetUIObject("HP")->GetComponent<UISprite>();
	hp_.SetSprite(gameUISprite->GetSprites()["hp"]);
	Vector3D green(0.0f, 1.0f, 0.0f);
	hp_.SetBarColor(green);

	//	CrossHair
	gameUISprite = uiData.GetUIObject("crossHair")->GetComponent<UISprite>();
	crossHair_.SetSprite(gameUISprite->GetSprites()["crossHair"]);

	//	Skills
		//	NormalBullet
	gameUISprite = uiData.GetUIObject("NormalCool")->GetComponent<UISprite>();
	nBulletSprite_.SetSprite(gameUISprite->GetSprites()["NormalAt"], gameUISprite->GetSprites()["Text"]);
	//	FiveBullet
	gameUISprite = uiData.GetUIObject("SlowCool")->GetComponent<UISprite>();
	slowAtCTSprite_.SetSprite(gameUISprite->GetSprites()["SlowAt"], gameUISprite->GetSprites()["Text"]);
	//	Dash
	gameUISprite = uiData.GetUIObject("DashCool")->GetComponent<UISprite>();
	avoidCTSprite_.SetSprite(gameUISprite->GetSprites()["Dash"], gameUISprite->GetSprites()["Text"]);
}

void Player::SetGameOverState(IGameState* gameOverState)
{
	pGameOverState_ = dynamic_cast<GameOverUI*>(gameOverState);
}
