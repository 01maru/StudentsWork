#include "PauseScreen.h"
#include "InputManager.h"
#include "UIManager.h"

void PauseScreen::Initialize()
{
    
}

void PauseScreen::Update()
{
    if (!active_) return;

    if (InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_SPACE)) isEnd_ = true;
}

void PauseScreen::Draw()
{
    if (!active_) return;

    for (auto& sprite : sprites_) {
        sprite.second.Draw();
    }
}

void PauseScreen::SetIsActive(bool active)
{
    if (active) InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
    active_ = active;
}

void PauseScreen::SetFileName(const std::string& filename)
{
    filename_ = filename;

    sprites_ = UIManager::GetInstance()->LoadFile(filename);

    for (auto& sprite : sprites_) {
        sprite.second.Update();
    }
}
