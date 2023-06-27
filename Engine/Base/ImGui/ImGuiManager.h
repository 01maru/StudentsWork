#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>

#include "Vector2D.h"
#include "Vector3D.h"

class ImGuiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

private:	//	関数
	ImGuiManager() {};
	~ImGuiManager() {};
public:
	static ImGuiManager* GetInstance();
	//	コピーコンストラクタ無効
	ImGuiManager(const ImGuiManager& obj) = delete;
	//	代入演算子無効
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	void Initialize();
	void Finalize();
	
	//	更新前処理
	void Begin();
	//	更新後処理
	void End();
	//	描画処理
	void Draw();

#pragma region Window

	void BeginWindow(const std::string& windowName);
	void BeginWindow(const std::string& windowName, bool menu);
	void EndWindow();

#pragma endregion

#pragma region Menu

	//	return isActive;
	bool BeginMenuBar();
	void EndMenuBar();

	//	return isActive;
	bool BeginMenu(const std::string& menuName);
	void EndMenu();

	//	MenuButton
	bool MenuItem(const std::string& name);

	bool TreeNode(const std::string& name);
	void TreePop();

#pragma endregion

#pragma region Input

	void SameLine();
	void Spacing();
	void Separator();

	bool SetButton(const std::string& buttonName, const Vector2D& size = Vector2D());
	void SetRadioButton(const std::string& buttonName, bool& flag);
	void InputFloat3(const std::string& name, Vector3D& vec);
	void InputFloat2(const std::string& name, Vector2D& vec);
	void SetSliderFloat(const std::string& sliderName, float& value, float spd = 1.0f, float minValue = 0.0f, float maxValue = 0.0f);
	void SetSliderFloat2(const std::string& sliderName, Vector2D& value, float spd = 1.0f, float minValue = 0.0f, float maxValue = 0.0f);
	void SetSliderFloat3(const std::string& sliderName, Vector3D& value, float spd = 1.0f, float minValue = 0.0f, float maxValue = 0.0f);
	void SetSliderInt(const std::string& sliderName, int& value, float spd, size_t minValue, size_t maxValue);
	void CheckBox(const std::string& name, bool& flag);
	void ColorPicker3(const std::string& name, Vector3D& color);

#pragma endregion

	void LabelText(const std::string& name, const std::string& text, float value);
	void Text(const char* fmt, ...);
	void InputText(const std::string& name, std::string& text, size_t strLen);

#pragma region Scroll

	//	return isActive;
	bool BeginChild(const Vector2D& size = Vector2D());
	void EndChild();

	void BeginGroup();
	void EndGroup();
	bool CollapsingHeader(const std::string& name);

#pragma endregion
};

