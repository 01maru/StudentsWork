﻿#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>
#include <vector>

#pragma comment(lib, "d3d12.lib")

class Shader;

class GPipeline
{
public:
	enum BlendMord
	{
		ADD_BLEND = 0,
		SUB_BLEND,
		INV_BLEND,
		ALPHA_BLEND,
		NONE_BLEND,
	};
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	ComPtr<ID3D12RootSignature> rootSignature_;
	ComPtr<ID3D12PipelineState> state_;

private:	//	関数
	void SetShader(Shader& shader);
	void SetBlendDesc(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, size_t mord = NONE_BLEND);
	void SetRootParamCBV(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, size_t shaderRegister, size_t registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootParamDescript(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE* pDescriptorRange, size_t numDescriptorRanges, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootSignature(size_t rootParamNum = 1, int32_t textureNum = 1);

public:
	void Initialize(Shader& shader, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, int32_t constBuffNum, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK depth_write_mask = D3D12_DEPTH_WRITE_MASK_ALL, bool isDeep = true, DXGI_FORMAT format = DXGI_FORMAT_R11G11B10_FLOAT, int32_t textureNum = 1);
	void Init(Shader& shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, int32_t constBuffNum, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK depth_write_mask = D3D12_DEPTH_WRITE_MASK_ALL, bool isDeep = true, DXGI_FORMAT format = DXGI_FORMAT_R11G11B10_FLOAT, int32_t textureNum = 1);
	void SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void SetGraphicsRootSignature();

	//	Setter
	void SetBlendMord(size_t mord);
};

