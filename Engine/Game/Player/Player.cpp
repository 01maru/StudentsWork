#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Quaternion.h"
#include "ImGuiManager.h"
#include "UISprite.h"
#include "GameScene.h"
#include <cassert>

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "QueryCallBack.h"
#include "XAudioManager.h"
#include "RayCast.h"

#include "PlayerIdleState.h"
#include "PlayerNoAttackState.h"
#include "GameCamera.h"

using namespace CollAttribute;
using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void Player::StatusInitialize()
{
	//	ステータスの読み込み
	HotReloadStatus();

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

	accTimer_.Initialize(accTime_, FALSE);
}

void Player::Initialize(MNE::IModel* model)
{
	Object3D::Initialize();
	SetModel(model);

	//	当たり判定設定
	float radius = 0.5f;
	offset_ = Vector3D(0.0f, radius, 0.0f);
	SetCollider(new SphereCollider(offset_, radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	//	ステートにPlayer設定
	PlayerMoveState::SetPlayer(this);
	PlayerAttackState::SetPlayer(this);

	StatusInitialize();

	moveVec_ = Vector3D(0, 0, -1);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void Player::CalcMoveVec(const MyMath::Vector2D& inputVec)
{
	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	Vector3D inputMoveVec = inputVec.y * camera->GetFrontVec() + inputVec.x * camera->GetRightVec();
	inputMoveVec.y = 0.0f;			//	平面上のベクトルにするためにY軸は常に0.0f

	//	現在の移動方向ベクトルと入力によるベクトルの内積
	float dot = moveVec_.dot(inputMoveVec);
	//	内積値の正規化
	const float MAX_DOT = 2.0f;
	dot = (-dot + 1.0f) / MAX_DOT;	//	範囲を0.0f~1.0fに変更し、移動量が大きいときに内積値が大きくなるように変更

	//	回転する角度を決める
	float rotAngle = ConvertToRad(mMax(dot * maxAngle_, minAngle_));
	float nowAngle = GetAngle(moveVec_, inputMoveVec);
	
	//	回転予定の角度より現在の角度が小さかったら(追い越さない用の処理)
	if (nowAngle <= rotAngle)
	{
		moveVec_ = inputMoveVec;
	}
	else
	{
		//	回転する向きを決める
		Vector3D cross = moveVec_.cross(inputMoveVec);
		//	外積のYの値がマイナスだったら反対方向回転
		if (cross.y < 0.0f)
		{
			rotAngle = -rotAngle;
		}

		//	Y軸回りに回転
		Vector3D axisY(0, 1, 0);
		Quaternion rotQ = MakeAxisAngle(axisY, rotAngle);
		//	ベクトルの回転
		moveVec_ = RotateVector(moveVec_, rotQ);
	}
}

void Player::CalcModelFront(const MyMath::Vector2D& inputVec)
{
	//	移動してなかったら以下の処理はしない
	if (isMoving_ == FALSE) return;

	//	移動方向ベクトル更新
	CalcMoveVec(inputVec);

	//	モデルの向き計算(変更予定)
	Vector3D axisZ(0, 0, -1);
	Vector3D axisX(-1, 0, 0);
	float dot = axisX.dot(moveVec_);
	mat_.angle_.y = GetAngle(axisZ, moveVec_);
	if (dot < 0) mat_.angle_.y = -mat_.angle_.y;
}

void Player::MaxSpdUpdate(float inputLen)
{
	//	移動してなかったら
	if (isMoving_ == FALSE)
	{
		nowMaxSpd_ = 0.0f;
		return;
	}

	//	走っていたら
	if (isRunning_ == TRUE)
	{
		nowMaxSpd_ = runSpd_;
	}
	//	歩いていたら
	else
	{
		float len = mMin(inputLen, 1.0f);
		nowMaxSpd_ = walkSpd_ * len;
	}

	//	空中にいたら減速させる
	if (onGround_ == FALSE) {
		nowMaxSpd_ *= jumpingDecSpd_;
	}
}

void Player::IsMovingUpdate()
{
	InputJoypad* joypad = InputManager::GetInstance()->GetPad();
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();
	int32_t frontInput = keyboard->GetKey(DIK_W) - keyboard->GetKey(DIK_S);
	int32_t sideInput = keyboard->GetKey(DIK_D) - keyboard->GetKey(DIK_A);

	//	パッドでの移動入力
	MyMath::Vector2D inputVec = joypad->GetThumbLNorm();
	//	パッド+キーでの入力
	inputVec += MyMath::Vector2D(sideInput, frontInput);

	float len = inputVec.GetLength();
	//	移動しているか判定
	bool prevIsMoving = isMoving_;
	isMoving_ = len != 0;

	accTimer_.SetIsIncrement(isMoving_);
	if (prevIsMoving != isMoving_)
	{
		decel_ = nowMaxSpd_ * decelRate_;

		if (accTimer_.GetIsActive() == FALSE)
		{
			accTimer_.StartCount();
		}
	}
	accTimer_.Update();

	MaxSpdUpdate(len);

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

void Player::CameraUpdate()
{
	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	//	頭の中心
	Vector3D target = mat_.trans_ + Vector3D(0.0f, 1.5f, 0.0f);
	//	スピードによる補間
	float offsetRate = mClamp(0.0f, 1.0f, spd_ / runSpd_);
	target -= moveVec_ * cameraOffset_.x * offsetRate;

	//	eye設定
	camera->SetEye(target - camera->GetDisEyeTarget() * camera->GetFrontVec());
	
	target += camera->GetFrontVec() * cameraOffset_.z;
	target.y += cameraOffset_.y * pCamera_->GetNormAngle();

	camera->SetTarget(target);

	camera->MatUpdate();
}

void Player::Update()
{
	//	HPバーのアニメーション更新
	hp_.Update();

	GetAnimation()->SetAnimeTimer(static_cast<float>(animationTimer_++));

	//	死亡していたら
	if (hp_.GetIsAlive() == false) {
		pGameScene_->ActiveGameOver();
		pGameScene_->SetNextState(GameScene::EndState);
		SetAnimationIdx("Death");
		SetAnimationTimer(0);
		GetAnimation()->SetIsLoop(false);
		GetAnimation()->SetAutoPlay(TRUE);
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

	CameraUpdate();

	crossHair_.Update(mat_.trans_ + offset_);

	attackState_->Update();

	rate_.Update();

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

void Player::HotReloadStatus()
{
	LoadData();

	avoidCTSprite_.SetMaxTime(avoidCoolTime_);
	slowAtCTSprite_.SetMaxTime(slowATCoolTime_);

	hp_.SetMaxHP(maxHP_);
}

void Player::ImGuiMenuUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (imgui->BeginMenuBar()) {
		if (imgui->BeginMenu("File")) {
			if (imgui->MenuItem("Load")) HotReloadStatus();
			if (imgui->MenuItem("Save")) SaveData();
			imgui->EndMenu();
		}
		imgui->EndMenuBar();
	}
}

void Player::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("PlayerStatus", true);

	ImGuiMenuUpdate();

	crossHair_.ImGuiUpdate();

	if (imgui->CollapsingHeader("Move")) {
		imgui->Text("IsMoving : %s", isMoving_ ? "TRUE" : "FALSE");
		imgui->Text("IsRunning : %s", isRunning_ ? "TRUE" : "FALSE");
		imgui->Text("Spd : %.2f", spd_);
		imgui->Text("MoveVec : (%.2f, %.2f, %.2f)", moveVec_.x, moveVec_.y, moveVec_.z);

		ImGuiMoveUpdate();
	}

	if (imgui->CollapsingHeader("Avoid")) {
		imgui->Text("AvoidIsActive : %s", avoidCTSprite_.GetIsActive() ? "TRUE" : "FALSE");
		imgui->Text("Avoid : %s", avoiding_ ? "TRUE" : "FALSE");

		ImGuiAvoidUpdate();
		//avoidCT_.ImGuiUpdate();
	}

	if (imgui->CollapsingHeader("Jump")) {
		imgui->Text("OnGround : %s", onGround_ ? "TRUE" : "FALSE");
		imgui->Text("MoveY : %.2f", moveY_);

		ImGuiJumpUpdate();
	}

	if (imgui->CollapsingHeader("Camera")) {
		ImGuiCamera();
		pCamera_->ImGuiUpdate();
	}

	if (imgui->CollapsingHeader("Model")) {
		imgui->Text("animationTimer : %d", animationTimer_);
		imgui->Text("angle : %.2f", mat_.angle_.y);
	}

	if (imgui->CollapsingHeader("Attack")) {
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

	if (imgui->CollapsingHeader("State")) {
		moveState_->ImGuiUpdate();
	}

	imgui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void Player::DrawUI()
{
	crossHair_.Draw();
	hp_.Draw();

	//	Skills
	nBulletSprite_.Draw();
	avoidCTSprite_.Draw();
	slowAtCTSprite_.Draw();
}

void Player::AddBullet(BulletInfo& bullet)
{
	bullets_.push_back(bullet);
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

float Player::GetNowMaxSpd()
{
	return nowMaxSpd_;
}

float Player::GetDecel()
{
	return decel_;
}

float Player::GetAccRate()
{
	return accTimer_.GetCountPerMaxCount();
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

std::list<BulletInfo>& Player::GetBullets()
{
	return bullets_;
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
	//	Sliding
	gameUISprite = uiData.GetUIObject("SlideCool")->GetComponent<UISprite>();
	avoidCTSprite_.SetSprite(gameUISprite->GetSprites()["Sliding"], gameUISprite->GetSprites()["Text"]);
}

void Player::SetGameScene(GameScene* gameScene)
{
	pGameScene_ = gameScene;
}

void Player::SetGameCamera(GameCamera* camera)
{
	pCamera_ = camera;
}
