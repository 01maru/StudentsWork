#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>

#include "Vector2D.h"
#include "Vector3D.h"

/**
* @file ImGuiManager.h
* @brief ImGuiの処理をまとめたファイル
*/

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

	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();

	/**
	* @fn Finalize()
	* exe終了時に呼び出す関数
	*/
	void Finalize();
	
	/**
	* @fn Begin()
	* 更新前処理関数
	*/
	void Begin();
	/**
	* @fn End()
	* 更新後処理関数
	*/
	void End();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
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
	bool SetRadioButton(const std::string& buttonName, int32_t& v, int32_t vButton);
	void InputFloat3(const std::string& name, Vector3D& vec);
	void InputFloat2(const std::string& name, Vector2D& vec);
	void InputFloat(const std::string& name, float& v);
	void InputInt(const std::string& name, int& v);
	void SetSliderFloat(const std::string& sliderName, float& value, float spd = 1.0f, float minValue = 0.0f, float maxValue = 0.0f);
	void SetSliderFloat2(const std::string& sliderName, Vector2D& value, float spd = 1.0f, float minValue = 0.0f, float maxValue = 0.0f);
	void SetSliderFloat3(const std::string& sliderName, Vector3D& value, float spd = 1.0f, float minValue = 0.0f, float maxValue = 0.0f);
	void SetSliderInt(const std::string& sliderName, int& value, float spd, size_t minValue, size_t maxValue);
	void CheckBox(const std::string& name, bool& flag);
	void ColorPicker3(const std::string& name, Vector3D& color);

#pragma endregion

	void LabelText(const std::string& name, const std::string& text, float value);
	void Text(const char* fmt, ...);
	bool InputText(const std::string& name, std::string& text);

	void PushID(int32_t id);
	void PopID();

#pragma region Scroll

	//	return isActive;
	bool BeginChild(const Vector2D& size = Vector2D());
	void EndChild();

	void BeginGroup();
	void EndGroup();
	bool CollapsingHeader(const std::string& name);

#pragma endregion
};

