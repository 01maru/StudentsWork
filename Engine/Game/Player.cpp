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

const float Player::MAX_SPD = 0.1f;
const int Player::INVINCIBLE_TIME = 90;

void Player::PlayerInitialize(IModel* model)
{
	Initialize();
	SetModel(model);
	float radius = 0.6f;
	SetCollider(new SphereCollider(Vector3D(0.0f, radius, 0.0f), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	//jumpSound = MyXAudio::GetInstance()->SoundLoadWave("jump.wav");
}

void Player::Update()
{
	InputManager* input = InputManager::GetInstance();
	
	int32_t front = input->GetKeyAndButton(DIK_W, InputJoypad::DPAD_Up) -
		input->GetKeyAndButton(DIK_S, InputJoypad::DPAD_Down);
	int32_t side = input->GetKeyAndButton(DIK_D, InputJoypad::DPAD_Right) -
		input->GetKeyAndButton(DIK_A, InputJoypad::DPAD_Left);

	ICamera* camera = CameraManager::GetInstance()->GetCamera();
	
	Vector3D moveVec;
	moveVec = (float)front * camera->GetFrontVec() + (float)side * camera->GetRightVec();
	moveVec.y = 0;
	
	bool isMove = front != 0 || side != 0;

	if (front != 0 && side != 0) {
		spd = MAX_SPD / 1.4142f;
	}
	else {
		spd = MAX_SPD;
	}

	if (!onGround) {
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		fallVec.y = MyMath::mMax (fallVec.y + fallAcc, fallVYMin);
		mat_.trans_ += fallVec;
	}
	else if (input->GetTriggerKeyAndButton(DIK_SPACE,InputJoypad::A_Button)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallVec = { 0.0f,jumpVYFist,0.0f };
		//MyXAudio::GetInstance()->SoundPlayWave(jumpSound, 0.1f);
	}

	moveVec *= spd;
	mat_.trans_ += moveVec;
	//camera->SetTarget({ mat.trans.x,mat.trans.y + 1.0f,mat.trans.z });
	//camera->EyeMove(moveVec);
	//camera->MatUpdate();

	if (isMove) {
		Vector3D axis(0, 0, -1);

		mat_.angle_.y = GetAngle(axis, moveVec);	//	atan2はダメ
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

	if (onGround) {
		const float adsDis = 0.2f;

		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.0f + adsDis)) {
			onGround = true;
			mat_.trans_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			ColliderUpdate();
			MatUpdate();
		}
		else {
			onGround = false;
			fallVec = {};
		}
	}
	else if (fallVec.y <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.0f)) {
			onGround = true;
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
