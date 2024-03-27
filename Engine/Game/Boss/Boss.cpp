#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "BossStartState.h"

#include "Player.h"
#include "Quaternion.h"

#include "ImGuiManager.h"

#include "IGameState.h"
#include "CollisionManager.h"

#include "BossBulletState.h"
#include "BossWayBullets.h"
#include "BossJumpAtState.h"
#include "BossBumpAtState.h"
#include "BossRockFallState.h"
#include "BossBeamState.h"
#include "BossTornadoState.h"
#include "BossRoarState.h"
#include "BossIdleState.h"
#include "BossDeathState.h"

using namespace MNE;
using namespace MyMath;
using namespace CollAttribute;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void Boss::StatusInitialize()
{
	hp_.SetMaxHP(maxHP_);

	//	初期ステート
	std::unique_ptr<BossState> next = std::make_unique<BossStartState>();
	SetCurrentState(next);

	BossData::LoadData();

	hp_.Initialize();
}

void Boss::Initialize(MNE::IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	BossState::SetBoss(this);

	StatusInitialize();

	float radius = 3.0f;
	MyMath::Vector3D offset(0.0f, 3.0f, 0.0f);
	SetCollider(new SphereCollider(offset, radius));
	collider_->SetAttribute(CollAttribute::COLLISION_ATTR_ENEMYS);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

float Boss::RotationUpdate()
{
	frontVec_ = mat_.trans_;
	frontVec_ -= player_->GetCenterPos();
	Vector2D vec2(frontVec_.x, frontVec_.z);
	float dis = frontVec_.GetLength();
	mat_.angle_.x = atan2(-frontVec_.y, vec2.GetLength());
	frontVec_.Normalize();
	mat_.angle_.y = atan2(frontVec_.x, frontVec_.z);

	return dis;
}

void Boss::CalcPriority(bool isClose, float normLen)
{
	float medV = mClamp(0.0f, 1.0f, normLen);
	float farV = 1.0f - medV;

	int32_t totalPriority = 0;
	int32_t priorityV = 0;

	for (int32_t i = 0; i < StateNum; i++)
	{
		if (isClose == TRUE)
		{
			priorityV = priority_.atState[i][nowForm_].range[AttackPriority::Close];
		}
		else
		{
			priorityV = static_cast<int32_t>(priority_.atState[i][nowForm_].range[AttackPriority::Medium] * medV);
			priorityV += static_cast<int32_t>(priority_.atState[i][nowForm_].range[AttackPriority::Long] * farV);
		}

		//	連続で同じ攻撃になる確率を減らす用
		if (i == prevAtState_)
		{
			float value = static_cast<float>(mMax(0, (maxConsecutiveNum_ - consecutiveAtState_)));
			priorityV = static_cast<int32_t>(priorityV * value / maxConsecutiveNum_);
		}

		totalPriority += priorityV;
		priority_.atState[i][nowForm_].range[AttackPriority::NowPriority] = priorityV;
	}

	int32_t rad = rand();
	rad = rad % totalPriority;

	int32_t nextState = NoAtState;
	for (int32_t i = 0; i < StateNum; i++)
	{
		int32_t nowPriority = priority_.atState[i][nowForm_].range[AttackPriority::NowPriority];
		rad -= nowPriority;

		if (rad <= 0 && nowPriority > 0)
		{
			nextState = i;
			break;
		}
	}

	//	次のステートへ
	SetCurrentState(nextState);

	SetAtState(nextState);
}

void Boss::Update()
{
	if (isActive_ == FALSE) return;

	//	死亡判定
	hp_.Update();

	currentState_->SetStateForSpecificSituation();

	if (stopState_ == FALSE) currentState_->Update();

	if (hp_.GetIsAlive() == true) {

		ColliderUpdate();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Collision
//-----------------------------------------------------------------------------

void Boss::CollisionUpdate()
{
	MatUpdate();
	collider_->Update();
}

void Boss::OnCollision(CollisionInfo& info)
{
	(void)info;
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void Boss::ImGuiMenuUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	if (imGui->BeginMenuBar()) {
		if (imGui->BeginMenu("File")) {
			if (imGui->MenuItem("Load")) LoadData();
			if (imGui->MenuItem("Save")) SaveData();
			imGui->EndMenu();
		}
		imGui->EndMenuBar();
	}
}

void Boss::ImGuiSelectState()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	if (imGui->CollapsingHeader("State")) {
		imGui->CheckBox("StopState", stopState_);

		int32_t prevState = nowState_;
		imGui->SetRadioButton("Bullet", nowState_, BulletState);
		imGui->SameLine();
		imGui->SetRadioButton("WayBullet", nowState_, WayBulletsState);
		imGui->SameLine();
		imGui->SetRadioButton("Beam", nowState_, BeamState);
		imGui->SameLine();
		imGui->SetRadioButton("JumpAt", nowState_, JumpAtState);

		imGui->SetRadioButton("Rock", nowState_, RockFallState);
		imGui->SameLine();
		imGui->SetRadioButton("Bump", nowState_, BumpState);
		imGui->SameLine();
		imGui->SetRadioButton("Tornado", nowState_, Tornado);

		imGui->SetRadioButton("Idol", nowState_, IdolState);
		imGui->SameLine();
		imGui->SetRadioButton("Start", nowState_, StartState);
		imGui->SameLine();
		imGui->SetRadioButton("Roar", nowState_, RoarState);
		imGui->SameLine();
		imGui->SetRadioButton("Death", nowState_, DeathState);

		//	ステート変更
		if (prevState != nowState_)
		{
			SetCurrentState(nowState_);
		}

		imGui->Text("BodyAttack : %s", bodyAt_ ? "TRUE" : "FALSE");

		if (imGui->TreeNode("AttackStateAI")) {
			imGui->Text("NowState : %d", nowState_);
			imGui->Text("prevAtState : %d", prevAtState_);
			imGui->Text("ConsecutiveAtState : %d", consecutiveAtState_);

			imGui->TreePop();
		}

		if (imGui->TreeNode("CurrentState")) {
			if (imGui->TreeNode("StateData")) {
				ImGuiStateUpdate(nowState_);

				imGui->TreePop();
			}
			currentState_->ImGuiUpdate();

			imGui->TreePop();
		}
	}
}

void Boss::ImGuiUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->BeginWindow("BossStatus", true);

	ImGuiMenuUpdate();

	ImGuiSelectState();

	if (imGui->CollapsingHeader("Model")) {
		//imGui->Text("animationTimer : %d", animationTimer_);
		imGui->Text("angle : %.2f", mat_.angle_.y);
		imGui->Text("frontVec : (%.2f, %.2f, %.2f)", frontVec_.x, frontVec_.y, frontVec_.z);
	}

	if (imGui->CollapsingHeader("HP")) {
		imGui->Text("isAlive : %s", hp_.GetIsAlive() ? "TRUE" : "FALSE");
		imGui->Text("HP : %d", hp_.GetHP());
		imGui->Text("NowForm(FirstForm : 0 SecondForm : 1) : %d", nowForm_);

		imGui->InputInt("MaxHP", maxHP_);
		hp_.SetMaxHP(maxHP_);
	}

	imGui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void Boss::DrawUI()
{
	hp_.Draw();
}

void Boss::AddBullet(EnemyBulletInfo& bullet)
{
	bullets_.push_back(bullet);
}

void Boss::DecHP(int32_t damage)
{
	hp_.DecHp(damage);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool Boss::GetIsAlive()
{
	return hp_.GetIsAlive();
}

bool Boss::GetIsHPLessThanHalf()
{
	int32_t halfHP = hp_.GetMaxHP() / 2;
	return hp_.GetHP() <= halfHP;
}

bool Boss::GetIsSecondForm()
{
	return nowForm_ == SecondForm;
}

bool Boss::GetBodyAttack()
{
	return bodyAt_;
}

MyMath::Vector3D* Boss::GetPositionPtr()
{
	return &mat_.trans_;
}

MyMath::Vector3D Boss::GetFrontVec()
{
	return frontVec_;
}

Player* Boss::GetPlayerPtr()
{
	return player_;
}

MyMath::Vector3D Boss::GetShotPoint()
{
	Vector3D offset(0.0f, 3.0f, 0.0f);
	return mat_.trans_ + offset;
}

MyMath::Vector3D Boss::GetBeamPoint()
{
	Vector3D offset(0.0f, 3.0f, 0.0f);
	return mat_.trans_ + offset;
}

std::list<EnemyBulletInfo>& Boss::GetBullets()
{
	return bullets_;
}

BeamInfo Boss::GetBeamInfo()
{
	return beam_;
}

GameScene* Boss::GetGameScene()
{
	return pGameScene_;
}

void Boss::SetAtState(int32_t atState)
{
	if (prevAtState_ == atState)
	{
		consecutiveAtState_++;
	}
	else {
		prevAtState_ = atState;
		consecutiveAtState_ = 0;
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Boss::SetNowForm(int32_t form)
{
	nowForm_ = form;
}

void Boss::SetIsActive(bool isActive)
{
	isActive_ = isActive;
}

void Boss::SetCurrentState(std::unique_ptr<BossState>& next)
{
	currentState_ = std::move(next);
	currentState_->Initialize();
}

void Boss::SetCurrentState(int32_t nextState)
{
	std::unique_ptr<BossState> next;
	if (nextState == Boss::BulletState) {
		next = std::make_unique<BossBulletState>();
	}
	else if (nextState == Boss::WayBulletsState) {
		next = std::make_unique<BossWayBullets>();
	}
	else if (nextState == Boss::BeamState) {
		next = std::make_unique<BossBeamState>();
	}
	else if (nextState == Boss::JumpAtState) {
		next = std::make_unique<BossJumpAtState>();
	}
	else if (nextState == Boss::RockFallState) {
		next = std::make_unique<BossRockFallState>();
	}
	else if (nextState == Boss::BumpState) {
		next = std::make_unique<BossBumpAtState>();
	}
	else if (nextState == Boss::Tornado) {
		next = std::make_unique<BossTornadoState>();
	}
	else if (nextState == Boss::IdolState) {
		next = std::make_unique<BossIdleState>();
	}
	else if (nextState == Boss::StartState) {
		next = std::make_unique<BossStartState>();
	}
	else if (nextState == Boss::RoarState) {
		next = std::make_unique<BossRoarState>();
	}
	else if (nextState == Boss::DeathState) {
		next = std::make_unique<BossDeathState>();
	}
	SetCurrentState(next);
}

void Boss::SetPlayer(Player* player)
{
	player_ = player;
}

void Boss::SetHPBarSprite(const MNE::Sprite& sprite)
{
	hp_.SetSprite(sprite);

	MyMath::Vector3D red(1.0f, 0.0f, 0.0f);
	hp_.SetBarColor(red);
}

void Boss::SetBodyAttack(bool attackFlag)
{
	bodyAt_ = attackFlag;
}

void Boss::SetBeamInfo(const BeamInfo& beam)
{
	beam_ = beam;
}

void Boss::SetGameScene(GameScene* gameScene)
{
	pGameScene_ = gameScene;
}
