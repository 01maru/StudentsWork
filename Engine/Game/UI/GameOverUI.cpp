#include "GameOverUI.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "CameraManager.h"
#include "GameOverCamera.h"
#include "Easing.h"

using namespace Easing;

void GameOverUI::Initialize()
{
	data_.LoadData("GameOver");
	fadeCounter_.Initialize(60, true);
}

void GameOverUI::Update()
{
	if (isActive_ == false) return;
	if (pCamera_->StartFade() == true &&
		fadeCounter_.GetFrameCount() == 0) {
		fadeCounter_.StartCount();
	}
	fadeCounter_.Update();
	bool select = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	data_.InputUpdate();

	if (select)
	{
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		if (data_.GetSelectName() == "Continue") {
			isActive_ = false;
		}

		if (data_.GetSelectName() == "Quit") {
			SceneManager::GetInstance()->SetNextScene("TITLESCENE");
			isActive_ = false;
		}
	}
	float color = lerp(0.0f, 1.0f, fadeCounter_.GetCountPerMaxCount());
	SceneManager::GetInstance()->ChangeScreenAlpha(color);

	data_.Update();
}

void GameOverUI::Draw()
{
	if (isActive_ == false) return;
	data_.Draw();
}

void GameOverUI::Start()
{
	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
	CameraManager* cameraMan = CameraManager::GetInstance();
	std::unique_ptr<ICamera> camera = std::make_unique<GameOverCamera>();
	camera->Initialize(cameraMan->GetMainCamera()->GetEye()
		, cameraMan->GetMainCamera()->GetTarget()
		, cameraMan->GetMainCamera()->GetUp());
	cameraMan->SetMainCamera(camera);

	pCamera_ = dynamic_cast<GameOverCamera*>(cameraMan->GetMainCamera());

	IGameState::Start();
}
