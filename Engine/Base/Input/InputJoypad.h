#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>

/**
* @file InputJoypad.h
* @brief コントローラーの入力処理をまとめたファイル
*/

class Vector2D;
namespace MNE
{

    class ImGuiManager;

    class InputJoypad
    {
    public:
        enum JoyPadButton
        {
            DPAD_Up    = XINPUT_GAMEPAD_DPAD_UP,    //  十字キー上
            DPAD_Down  = XINPUT_GAMEPAD_DPAD_DOWN,  //  十字キー下
            DPAD_Left  = XINPUT_GAMEPAD_DPAD_LEFT,  //  十字キー左
            DPAD_Right = XINPUT_GAMEPAD_DPAD_RIGHT, //  十字キー右
            START_Button  = XINPUT_GAMEPAD_START,        //  横三本線
            BACK_Button   = XINPUT_GAMEPAD_BACK,         //  長方形x2
            LStick_Button = XINPUT_GAMEPAD_LEFT_THUMB,      //  Lスティック押し込み
            RStick_Button = XINPUT_GAMEPAD_RIGHT_THUMB,     //  Rスティック押し込み
            Left_Button  = XINPUT_GAMEPAD_LEFT_SHOULDER,
            Right_Button = XINPUT_GAMEPAD_RIGHT_SHOULDER,
            A_Button = XINPUT_GAMEPAD_A,
            B_Button = XINPUT_GAMEPAD_B,
            X_Button = XINPUT_GAMEPAD_X,
            Y_Button = XINPUT_GAMEPAD_Y,
        };
    private:
        //  padが有効か
        bool active_ = false;
        bool inputted_ = false;
        //  Info
        XINPUT_STATE state_{};
        XINPUT_STATE prevState_{};
        bool activeVibration_ = false;
        XINPUT_VIBRATION vibration_{};

    private:    //  関数
        void SetDeadZone(int16_t& sThumb, int32_t deadzone);

        void ImGuiUpdateVibration(ImGuiManager* imgui);
        void ImGuiUpdateButton(ImGuiManager* imgui);
        void ImGuiUpdateDPAD(ImGuiManager* imgui);
        void ImGuiUpdateStick(ImGuiManager* imgui);
        void ImGuiUpdateTrigger(ImGuiManager* imgui);
    public:
        /**
        * @fn Update()
        * 更新処理関数
        */
        void Update();
        /**
        * @fn Update()
        * ImGui更新処理関数
        */
        void ImGuiUpdate();

    #pragma region Getter

        bool GetIsActive() { return active_; }
        bool GetInputted() { return inputted_; }

        bool GetButton(JoyPadButton button);
        bool GetButtonTrigger(JoyPadButton button);
        bool GetButtonRelease(JoyPadButton button);

        bool GetLTrigger();
        bool GetRTrigger();
        size_t GetLTriggerValue();
        size_t GetRTriggerValue();

        Vector2D GetThumbR();
        bool GetTriggerThumbRX();
        bool GetTriggerThumbRY();
        Vector2D GetThumbL();
        bool GetTriggerThumbLX();
        bool GetTriggerThumbLY();
        int32_t GetMaxThumbRange() { return 32767; }
        int32_t GetThumbRDeadZone();

    #pragma endregion
    
    #pragma region Setter

        //  motorSpd(x, y) = (Left, Right)
        //  min ~ max = 0 ~ 65535
        /**
        * @fn SetVibration(const Vector2D&)
        * バイブレーションの強さ設定用関数
        * @param motorSpd バイブレーションの強さ motorSpd(x, y) = (Left, Right)
        */
        void SetVibration(const Vector2D& motorSpd);

    #pragma endregion
    };

}
