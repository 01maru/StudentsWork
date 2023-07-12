#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include <cassert>
#include "Quaternion.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "QueryCallBack.h"
#include "InputJoypad.h"
#include "XAudioManager.h"

using namespace CollAttribute;

void Player::StaminaUpdate()
{
	if (staminaHealTimer_ > 0) {
		staminaHealTimer_--;
	}
	else {
		stamina_ = MyMath::mMin((float)MAX_STAMINA, stamina_ + STAMINA_HEAL);
	}
}

void Player::AvoidUpdate(InputManager* input)
{
	if (isAvoid_)		return;
	if (stamina_ <= 0)	return;
	if (!onGround_)		return;
	if (!input->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) return;

	if (input->GetTriggerKeyAndButton(DIK_LSHIFT, InputJoypad::B_Button)) {
		isAvoid_ = true;
		spd_ = INVINCIBLE_SPD;
		avoidTimer_ = INVINCIBLE_TIME;

		//	stamina
		stamina_ = MyMath::mMax(stamina_ - AVOID_STAMINA, 0.0f);
		staminaHealTimer_ = STAMINA_HEAL_TIME;
		
		//	playDashwav
	}

	if (avoidTimer_-- <= 0) { isAvoid_ = false; }
}

Vector3D Player::CalcMoveVec(InputKeyboard* keyboard, InputJoypad* pad, ICamera* camera)
{
	Vector3D moveVec;

	if (isAvoid_) {
		moveVec = Vector3D(modelFrontVec_.x, 0.0f, modelFrontVec_.y) * spd_;

		return moveVec;
	}

	int32_t frontKey = keyboard->GetKey(DIK_W) - keyboard->GetKey(DIK_S);
	int32_t sideKey = keyboard->GetKey(DIK_D) - keyboard->GetKey(DIK_A);

	Vector2D inputVec = pad->GetThumbL().GetNormalize() + Vector2D(sideKey, frontKey);
	moveVec = inputVec.y * camera->GetFrontVec() + inputVec.x * camera->GetRightVec();
	moveVec.y = 0;

	spd_ = MAX_SPD;

	return moveVec;
}

void Player::CalcFallSpd(InputManager* input)
{
	if (!onGround_) {
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		fallVec_.y = MyMath::mMax(fallVec_.y + fallAcc, fallVYMin);
	}
	else if (input->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
		onGround_ = false;
		const float jumpVYFist = 0.2f;
		fallVec_ = { 0.0f,jumpVYFist,0.0f };

		//	stamina
		stamina_ = MyMath::mMax(stamina_ - JUMP_STAMINA, 0.0f);
		staminaHealTimer_ = STAMINA_HEAL_TIME;
		//	playJumpwav
	}
}

void Player::Initialize(IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	float radius = 0.6f;
	SetCollider(new SphereCollider(Vector3D(0.0f, radius, 0.0f), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	hp_ = MAX_HP;
	stamina_ = (float)MAX_STAMINA;
}

void Player::Update()
{
	InputManager* input = InputManager::GetInstance();
	InputKeyboard* keyboard = input->GetKeyboard();
	InputJoypad* pad = input->GetPad();
	ICamera* camera = CameraManager::GetInstance()->GetCamera();

	if (hp_ <= 0) isAlive_ = false;

	if (!isAlive_) return;

	StaminaUpdate();

	AvoidUpdate(input);
	
	Vector3D moveVec = CalcMoveVec(keyboard, pad, camera);
	
	CalcFallSpd(input);

	//	本移動
	mat_.trans_ += moveVec * spd_ + fallVec_;
	camera->SetTarget({ mat_.trans_.x,mat_.trans_.y + 1.0f,mat_.trans_.z });
	camera->MatUpdate();

	if (moveVec.GetLength() != 0.0f) {
		Vector3D axis(0, 0, -1);

		mat_.angle_.y = GetAngle(axis, moveVec);
	}

	ColliderUpdate();
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

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	mat_.trans_ += callback.move;

	MatUpdate();
	collider->Update();

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
			fallVec_ = {};
		}
	}
	else if (fallVec_.y <= 0.0f) {
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
	//mat.trans.x -= camera->GetFrontVec().x;
	//mat.trans.z -= camera->GetFrontVec().z;
	MatUpdate();
}
