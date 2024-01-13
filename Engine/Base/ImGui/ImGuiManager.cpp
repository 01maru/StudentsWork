#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <imgui_stdlib.h>
#include "Window.h"
#include "DirectX.h"

using namespace MNE;

MNE::ImGuiManager* MNE::ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void MNE::ImGuiManager::Initialize()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Window::GetInstance()->GetHwnd());

	MyDirectX* dx = MyDirectX::GetInstance();
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT result = dx->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));
	ImGui_ImplDX12_Init(dx->GetDev(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
}

void MNE::ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void MNE::ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MNE::ImGuiManager::End()
{
	ImGui::Render();
}

void MNE::ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

void MNE::ImGuiManager::BeginWindow(const std::string& windowName)
{
	ImGui::Begin(windowName.c_str());
}

void MNE::ImGuiManager::BeginWindow(const std::string& windowName, bool menu)
{
	ImGuiWindowFlags flag = 0;

	if (menu) flag = ImGuiWindowFlags_MenuBar;
	ImGui::Begin(windowName.c_str(), 0, flag);
}

void MNE::ImGuiManager::EndWindow()
{
	ImGui::End();
}

bool MNE::ImGuiManager::BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}

void MNE::ImGuiManager::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool MNE::ImGuiManager::BeginMenu(const std::string& menuName)
{
	return ImGui::BeginMenu(menuName.c_str());
}

void MNE::ImGuiManager::EndMenu()
{
	ImGui::EndMenu();
}

bool MNE::ImGuiManager::MenuItem(const std::string& name)
{
	return ImGui::MenuItem(name.c_str());
}

bool MNE::ImGuiManager::TreeNode(const std::string& name)
{
	return ImGui::TreeNode(name.c_str());
}

void MNE::ImGuiManager::TreePop()
{
	ImGui::TreePop();
}

void MNE::ImGuiManager::SameLine()
{
	ImGui::SameLine();
}

void MNE::ImGuiManager::Spacing()
{
	ImGui::Spacing();
}

void MNE::ImGuiManager::Separator()
{
	ImGui::Separator();
}

bool MNE::ImGuiManager::SetButton(const std::string& buttonName, const Vector2D& size)
{
	return ImGui::Button(buttonName.c_str(), ImVec2(size.x, size.y));
}

bool MNE::ImGuiManager::SetRadioButton(const std::string& buttonName, int32_t& v, int32_t vButton)
{
	return ImGui::RadioButton(buttonName.c_str(), &v, vButton);
}

void MNE::ImGuiManager::InputFloat3(const std::string& name, Vector3D& vec)
{
	float v[3] = { vec.x,vec.y,vec.z };
	ImGui::InputFloat3(name.c_str(), v);

	vec = { v[0],v[1],v[2] };
}

void MNE::ImGuiManager::InputFloat2(const std::string& name, Vector2D& vec)
{
	float v[2] = { vec.x,vec.y };
	ImGui::InputFloat2(name.c_str(), v);

	vec = { v[0],v[1] };
}

void MNE::ImGuiManager::InputFloat(const std::string& name, float& v)
{
	ImGui::InputFloat(name.c_str(), &v);
}

void MNE::ImGuiManager::InputInt(const std::string& name, int& v)
{
	ImGui::InputInt(name.c_str(), &v);
}

void MNE::ImGuiManager::SetSliderFloat(const std::string& sliderName, float& value, float spd, float minValue, float maxValue)
{
	//ImGui::SliderFloat(sliderName.c_str(), &value, minValue, maxValue);
	ImGui::DragFloat(sliderName.c_str(), &value, spd, minValue, maxValue);
}

void MNE::ImGuiManager::SetSliderFloat2(const std::string& sliderName, Vector2D& value, float spd, float minValue, float maxValue)
{
	float v[2] = { value.x,value.y };
	ImGui::DragFloat2(sliderName.c_str(), v, spd, minValue, maxValue);

	value = { v[0],v[1] };
}

void MNE::ImGuiManager::SetSliderFloat3(const std::string& sliderName, Vector3D& value, float spd, float minValue, float maxValue)
{
	float v[3] = { value.x,value.y,value.z };
	ImGui::DragFloat3(sliderName.c_str(), v, spd, minValue, maxValue);

	value = { v[0],v[1],v[2] };
}

void MNE::ImGuiManager::SetSliderInt(const std::string& sliderName, int& value, float spd, size_t minValue, size_t maxValue)
{
	ImGui::DragInt(sliderName.c_str(), &value, spd, (int)minValue, (int)maxValue);
}

void MNE::ImGuiManager::CheckBox(const std::string& name, bool& flag)
{
	ImGui::Checkbox(name.c_str(), &flag);
}

void MNE::ImGuiManager::ColorPicker3(const std::string& name, Vector3D& color)
{
	float v[3] = { color.x,color.y,color.z };

	ImGui::ColorPicker3(name.c_str(), v);
	color = { v[0],v[1],v[2] };
}

void MNE::ImGuiManager::LabelText(const std::string& name, const std::string& text, float value)
{
	ImGui::LabelText(name.c_str(), text.c_str(), value);
}

void MNE::ImGuiManager::Text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	ImGui::TextV(fmt, args);
	va_end(args);
}

bool MNE::ImGuiManager::InputText(const std::string& name, std::string& text)
{
	return ImGui::InputText(name.c_str(), &text);
}

void MNE::ImGuiManager::PushID(int32_t id)
{
	ImGui::PushID(id);
}

void MNE::ImGuiManager::PopID()
{
	ImGui::PopID();
}

bool MNE::ImGuiManager::BeginChild(const Vector2D& size)
{
	return ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(size.x, size.y), ImGuiWindowFlags_NoTitleBar);
}

bool MNE::ImGuiManager::BeginChild(int64_t childId, const Vector2D& size)
{
	return ImGui::BeginChild(ImGui::GetID((void*)childId), ImVec2(size.x, size.y), ImGuiWindowFlags_NoTitleBar);
}

void MNE::ImGuiManager::EndChild()
{
	ImGui::EndChild();
}

void MNE::ImGuiManager::BeginGroup()
{
	ImGui::BeginGroup();
}

void MNE::ImGuiManager::EndGroup()
{
	ImGui::EndGroup();
}

bool MNE::ImGuiManager::CollapsingHeader(const std::string& name)
{
	return ImGui::CollapsingHeader(name.c_str());
}
